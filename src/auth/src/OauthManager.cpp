#include <iostream>
#include <utility>

#include <aws/core/client/ClientConfiguration.h>

#include "auth/OauthManager.h"
#include "auth/PkceManager.h"
#include "auth/interfaces/IStateHashManager.h"
#include "auth/interfaces/ITokenRequestManager.h"
#include "auth/SessionManager.h"

#include "webutil/path.h"
#include "webutil/HttpSender.h"
#include "auth/StateHashManager.h"
#include "auth/TokenRequestManager.h"
#include "parse/util.h"
#include "config/poe_auth.h"

namespace auth {
    OauthManager::OauthManager() :
            OauthManager{std::make_unique<PkceManager>(),
                         std::make_unique<StateHashManager>(),
                         std::make_unique<TokenRequestManager>(),
                         std::make_unique<AuthCodeManager>(),
                         std::make_unique<SessionManager>()} {}
    
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
/// @return The invocation response payload to send back to the user.
    std::string auth::OauthManager::start_auth() {
        auto code_challenge = pkce_manager_->get_code_challenge();
        auto code_verifier = pkce_manager_->get_code_verifier();
        auto state_hash = state_hash_manager_->get_state_hash();
        auto session_id = session_manager_->get_session_token();
        
        std::cout << "Session ID: " << session_id << std::endl;
        
        // Set up connection to Oauth Database
        Aws::Client::ClientConfiguration client_configuration;
        client_configuration.region = "us-west-1";
        axel::Database oauth_database{config::axel::database::auth, client_configuration};
        
        oauth_database.put({{"session_id", session_id},
                            {"state_hash", state_hash},
                            {"code_verifier", code_verifier}});
        
        std::string auth_url = auth_code_manager_->get_auth_url(code_challenge, state_hash);
        // Construct invocation response
        std::unordered_map<std::string, std::string> headers = {{"Location", auth_url},
                                                                {"Set-Cookie", session_id}};
        auto response = parse::make_invocation_response_payload(302, std::move(headers), "");
        
        set_state(State::USER_REDIRECTED);
        
        return response;
    }

/// Processes the authorization response from the authorization server after the user has logged in and approved
/// the required scopes.
/// @param query_string The query_string string containing the query parameters "state" and "code", returned from
/// the authorization server after a successful login by the user.
/// @param session_id The session_id identifying the authentication request.
/// @return The session token for tracking this user session.
/// @note The user session is considered established once the token is sent out.
    std::string auth::OauthManager::receive_auth(const std::string& query_string, const std::string& session_id) {
        std::unordered_map<std::string, std::string> query_params = webutil::extract_query_params(query_string);
        auto state_hash = query_params["state"];
        auto auth_code = query_params["code"];
        
        if (state_hash_manager_->check_state_hash(session_id, state_hash)) {
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