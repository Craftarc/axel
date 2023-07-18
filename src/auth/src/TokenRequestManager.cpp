#include <boost/beast/http.hpp>

#include "src/auth/include/TokenRequestManager.h"
#include "config/poe_auth_config.h"
#include "webutil/http.h"
#include "webutil/path.h"
#include "config/axel.h"

namespace webutil {
    /// @param The authorization code to construct the request from.
    TokenRequestManager::TokenRequestManager() {}
    
    /// @return The access token HTTP request.
    http::request<http::string_body> TokenRequestManager::get_token_request() const {
        return token_request_;
    }
    
    
    /// @param auth_code The authorization code to be used in the request.
    /// @return The fully constructed HTTP POST request.
    namespace http = boost::beast::http;
    
    http::request<http::string_body>
    TokenRequestManager::make_token_request(std::string auth_code, std::string code_verifier) {
        // Check input validity
        if (auth_code == "") {
            throw std::runtime_error("auth_code is empty");
        } else if (code_verifier == "") {
            throw std::runtime_error("code_verifier is empty");
        }
        
        std::string request_body = pathutil::make_form_data({{"client_id", config::axel::client_id},
                                                             {"client_secret", "secret"},
                                                             {"grant_type", config::axel::grant_type},
                                                             {"code", std::move(auth_code)},
                                                             {"redirect_uri", config::poe::paths::redirect_uri},
                                                             {"scope", config::poe::scopes::account::profile},
                                                             {"code_verifier", std::move(code_verifier)}});
        
        return make_http_request("POST",
                                 config::poe::paths::auth_path,
                                 {{"host", config::poe::host},
                                  {"content-type",
                                   "application/x-www-form-urlencoded"}},
                                 request_body);
    }
}
