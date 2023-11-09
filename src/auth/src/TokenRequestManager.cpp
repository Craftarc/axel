#include "auth/TokenRequestManager.h"

#include <iostream>

#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <spdlog/spdlog.h>

#include "config/axel.h"
#include "config/poe.h"
#include "util/http.h"
#include "util/interfaces/IHttpSender.h"
#include "util/path.h"

auth::TokenRequestManager::TokenRequestManager() = default;

namespace http = boost::beast::http;

/// Constructs and sends out the token exchange request. The tokens are then parsed from the response.
/// @param auth_code The authorization code to be included in the token exchange request.
/// @param code_verifier The code verifier to be included in the token exchange request.
/// @param http_sender The HttpSender class that defines the request-sending behaviour and underlying networking.
/// @return The access token
std::string
auth::TokenRequestManager::send_token_request(std::string auth_code,
                                              std::string code_verifier,
                                              std::shared_ptr<util::IHttpSender>
                                              http_sender) const {
    const int MAX_RESPONSE_BODY = 10;

    namespace scopes = config::poe::scopes;

    // Check input validity
    if (auth_code.empty()) {
        throw std::runtime_error("auth_code is empty");
    } else if (code_verifier.empty()) {
        throw std::runtime_error("code_verifier is empty");
    }

    std::string request_body = util::make_form_data(
    { { "client_id", config::axel::client_id },
      { "client_secret", "xJqwozXycup5" },
      { "grant_type", config::axel::grant_type },
      { "code", std::move(auth_code) },
      { "redirect_uri", config::poe::paths::redirect_uri },
      { "scope",
        util::concatenate_with_space({ scopes::profile,
                                       scopes::stashes,
                                       scopes::characters,
                                       scopes::item_filter,
                                       scopes::league_accounts }) },
      { "code_verifier", std::move(code_verifier) } });

    // Add the headers to the request
    auto full_request =
    util::make_http_request("POST",
                            config::poe::paths::token,
                            { { "host", config::poe::host },
                              { "content-type",
                                "application/x-www-form-urlencoded" } },
                            request_body);

    full_request.prepare_payload();  // Automatically set Content-Length
    auto response_body = http_sender->send_http_request(full_request,
                                                        MAX_RESPONSE_BODY);

    spdlog::info("Response: {}", response_body);

    // Extract access token
    boost::json::value json = boost::json::parse(response_body);
    auto access_token = json.at("access_token").as_string().c_str();

    return access_token;
}
