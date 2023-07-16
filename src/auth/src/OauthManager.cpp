#include <iostream>
#include <utility>

#include "src/auth/include/OauthManager.h"
#include "webutil/path.h"
#include "webutil/hash.h"
#include "config/poe_auth_config.h"
#include "webutil/http.h"

namespace {
    // Declarations
    // Definitions
    std::string concatenate_with_space(const std::vector<std::string>& string_list) {
        std::string result;
        for (auto it = std::begin(string_list); it != std::end(string_list); ++it) {
            // If iterator is at last element of vector don't add a trailing space
            if (it == std::prev(string_list.end())) {
                result += *it;
            } else {
                result += *it + "%20"; // ASCII encoding for whitespace
            }
        }
        
        return result;
    }
}

namespace webutil {
    OauthManager::OauthManager() : pkce_manager_(PkceManager{}),
                                   state_hash_manager_(StateHashManager{}),
                                   token_request_manager_(TokenRequestManager{}),
                                   state_(State::INITIAL) {
        authorization_url_ = make_authorization_url();
    }
    
    OauthManager::OauthManager(PkceManager pkce_manager, StateHashManager state_hash_manager,
                               TokenRequestManager token_request_manager)
            : pkce_manager_(std::move(pkce_manager)), state_hash_manager_(std::move(state_hash_manager)),
              token_request_manager_(token_request_manager), state_(State::INITIAL) {
        authorization_url_ = make_authorization_url();
    }
    
    void OauthManager::send_token_request() {
        std::string response = send_http_request(token_request_);
        std::cout << response;
    }
    
    std::string OauthManager::get_state_hash() const {
        return OauthManager::state_hash_manager_.get_state_hash();
    }
    
    /* Constructs each query parameter one by one then calls add_query_parameters
     * to add it all to the auth url */
    std::string OauthManager::make_authorization_url() {
        
        namespace scopes = config::poe::scopes;
        
        
        // We request all scopes
        std::string scope = concatenate_with_space({scopes::account::profile,
                                                    scopes::account::stashes,
                                                    scopes::account::characters,
                                                    scopes::account::league_accounts,
                                                    scopes::account::item_filter});
        std::string endpoint = config::poe::host + config::poe::paths::auth_path;
        
        std::unordered_map<std::string, std::string> query_map({{"client_id", "axel"},
                                                                {"response_type", "code"},
                                                                {"scope", scope},
                                                                {"state", state_hash_manager_.get_state_hash()},
                                                                {"redirect_uri", config::poe::paths::redirect_uri},
                                                                {"code_challenge", pkce_manager_.get_code_challenge()},
                                                                {"code_challenge_method", "S256"}});
        
        return pathutil::add_query_parameters(endpoint, query_map);
    }
    
    std::string OauthManager::get_authorization_url() {
        state_ = State::AUTH_SENT;
        return authorization_url_;
    }
    
    namespace http = boost::beast::http;
    
    void OauthManager::make_token_request() {
        if (state_ != State::AUTH_OK) {
            throw std::runtime_error("Cannot make token request: Authorization code has not been received.");
        } else {
            token_request_ = token_request_manager_.make_token_request(authorization_code_,
                                                                       pkce_manager_.get_code_verifier()); // Delegate the construction of the request
            state_ = State::TOKEN_REQUEST_MADE;
        }
    }
    
    void OauthManager::receive_authorization_code(const std::string& url_string) {
        if (state_ != State::AUTH_SENT) {
            throw std::runtime_error("Cannot receive authorization code: Authorization request has not been sent.");
        }
        auto query_params = pathutil::extract_query_params(url_string);
        
        // Check if keys exist in map
        auto state_it = query_params.find("state");
        auto auth_code_it = query_params.find("code");
        
        if (state_it != query_params.end()) {
            auto state = state_it->second;
        } else {
            throw std::runtime_error("'state' query parameter not found in authorization url_string");
        }
        
        if (auth_code_it != query_params.end()) {
            auto auth_code = auth_code_it->second;
            
            if (state_hash_manager_.verify_state_hash(query_params["state"])) { // Verify state hash matches
                state_ = State::AUTH_OK;
                authorization_code_ = auth_code;
            } else {
                throw std::runtime_error(
                        "'WARNING: State returned by authorization server does not match generated state'");
            }
        } else {
            throw std::runtime_error("'code' query parameter not found in authorization url_string");
        }
    }
} // webutil