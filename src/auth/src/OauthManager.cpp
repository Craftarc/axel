#include <iostream>
#include <utility>

#include "auth/OauthManager.h"
#include "auth/PkceManager.h"
#include "auth/interfaces/IStateHashManager.h"
#include "auth/interfaces/ITokenRequestManager.h"
#include "auth/SessionManager.h"

#include "webutil/path.h"
#include "webutil/HttpSender.h"

namespace auth {
    
    OauthManager::OauthManager(std::unique_ptr<PkceManager> pkce_manager,
                               std::unique_ptr<IStateHashManager> state_hash_manager,
                               std::unique_ptr<ITokenRequestManager> token_request_manager,
                               std::unique_ptr<AuthCodeManager> auth_code_manager,
                               std::unique_ptr<SessionManager> session_manager)
            : pkce_manager_(std::move(pkce_manager)),
              state_hash_manager_(std::move(state_hash_manager)),
              token_request_manager_(std::move(token_request_manager)),
              auth_code_manager_(std::move(auth_code_manager)),
              session_manager_(std::move(session_manager)),
              state_(State::INITIAL) {};

/// Responsible for calling functions and methods in sequence according to the Oauth specification.
/// @return The authorization url string for user to visit
    std::string auth::OauthManager::start_auth() {
        auto code_challenge = pkce_manager_->get_code_challenge();
        auto state_hash = state_hash_manager_->get_state_hash();
        set_state(State::USER_REDIRECTED);
        return auth_code_manager_->get_auth_url(code_challenge, state_hash);
    }

/// Processes the authorization response from the authorization server after the user has logged in and approved
/// the required scopes.
/// @param query_string The query_string string containing the query parameters "state" and "code", returned from
/// the authorization server after a successful login by the user.
/// @return The session token for tracking this user session.
/// @note The user session is considered established once the token is sent out.
    std::string auth::OauthManager::receive_auth(const std::string& query_string) {
        std::unordered_map<std::string, std::string> query_params = webutil::extract_query_params(query_string);
        auto state_hash = query_params["state"];
        auto auth_code = query_params["code"];
        
        if (state_hash_manager_->check_state_hash(state_hash)) {
            set_state(State::AUTH_CODE_RECEIVED);
        }
        
        if (state_ != State::AUTH_CODE_RECEIVED) {
            throw std::runtime_error("State hash does not match.");
        } else {
            std::string code_verifier = pkce_manager_->get_code_verifier();
            auto tokens = token_request_manager_->send_token_request(auth_code,
                                                                     code_verifier,
                                                                     std::make_unique<webutil::HttpSender>());
            set_state(State::TOKENS_RECEIVED);
            std::string session_token = session_manager_->get_session_token();
            set_state(State::SESSION_ESTABLISHED);
            return session_token;
        }
    }
    
    void auth::OauthManager::set_state(OauthManager::State state) {
        state_ = state;
    }
} // auth