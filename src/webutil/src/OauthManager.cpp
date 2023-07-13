#include <iostream>
#include <botan/base64.h>
#include "webutil/OauthManager.h"
#include "webutil/pathutil.h"
#include "config/poe_auth_config.h"
#include "webutil/hash.h"

namespace {
    // Declarations
    std::string concatenate_with_space(const std::vector<std::string>& string_list);
    
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
    
    // Lifetime extension
    std::string make_state_hash() {
        std::vector<uint8_t> secret_bytes = webutil::generate_secret_bytes();
        auto base64_state_hash = Botan::base64_encode(secret_bytes.data(), secret_bytes.size());
        return webutil::base64_url_encode(base64_state_hash);
    }
}

namespace webutil {
    OauthManager::OauthManager(const PkceManager& pkce_manager)
            : pkce_manager_(pkce_manager), state_hash_(make_state_hash()) {
        authorization_url_ = make_authorization_url();
    }
    
    /* Constructs each query parameter one by one then calls add_query_parameters
     * to add it all to the auth url */
    std::string OauthManager::make_authorization_url() {
        using namespace config::poe;
        
        // We request all scopes
        std::string scopes = concatenate_with_space({scopes::account::profile, scopes::account::stashes,
                                                     scopes::account::characters, scopes::account::league_accounts,
                                                     scopes::account::item_filter});
        std::string endpoint = config::poe::host + config::poe::paths::auth_path;
        
        std::map<std::string, std::string> query_map({{"client_id", "axel"},
                                                      {"response_type", "code"},
                                                      {"scope", scopes},
                                                      {"state", state_hash_},
                                                      {"redirect_uri", paths::redirect_uri},
                                                      {"code_challenge", pkce_manager_.get_code_challenge()},
                                                      {"code_challenge_method", "S256"}});
        
        return pathutil::add_query_parameters(endpoint, query_map);
    }
    
    std::string OauthManager::get_authorization_url() {
        return authorization_url_;
    }
    
    std::string OauthManager::get_state_hash() {
        return state_hash_;
    }
} // webutil