#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <iostream>

#include "auth/TokenRequestManager.h"
#include "config/poe_auth.h"
#include "webutil/http.h"
#include "webutil/path.h"
#include "config/axel.h"
#include "webutil/interfaces/IHttpSender.h"

/// @param The authorization code to construct the request from.
auth::TokenRequestManager::TokenRequestManager() {}

/// @param auth_code The authorization code to be used in the request.
/// @return A string pair in the following format: {access_token, refresh_token}
namespace http = boost::beast::http;

/// Constructs and sends out the token exchange request. The tokens are then parsed from the response.
/// @param auth_code The authorization code to be included in the token exchange request.
/// @param code_verifier The code verifier to be included in the token exchange request.
/// @param http_sender The HttpSender class that defines the request-sending behaviour and underlying networking.
/// @return A pair of strings in the format: {${access_token}, ${refresh_token}}
std::pair<std::string, std::string>
auth::TokenRequestManager::send_token_request(std::string auth_code,
                                              std::string code_verifier,
                                              std::unique_ptr<webutil::IHttpSender> http_sender) const {
    // Check input validity
    if (auth_code == "") {
        throw std::runtime_error("auth_code is empty");
    } else if (code_verifier == "") {
        throw std::runtime_error("code_verifier is empty");
    }
    
    std::string request_body = webutil::make_form_data({{"client_id", config::axel::client_id},
                                                        {"client_secret", "secret"},
                                                        {"grant_type", config::axel::grant_type},
                                                        {"code", std::move(auth_code)},
                                                        {"redirect_uri", config::poe::paths::redirect_uri},
                                                        {"scope", config::poe::scopes::profile},
                                                        {"code_verifier", std::move(code_verifier)}});
    
    auto full_request = webutil::make_http_request("POST",
                                                   config::poe::paths::auth_path,
                                                   {{"host", config::poe::host},
                                                    {"content-type",
                                                     "application/x-www-form-urlencoded"}},
                                                   request_body);
    
    auto response_body = http_sender->send_http_request(full_request, 0);
    
    boost::json::value json = boost::json::parse(response_body);
    auto access_token = json.at("access_token").as_string().c_str();
    auto refresh_token = json.at("refresh_token").as_string().c_str();
    
    return {access_token, refresh_token};
}


