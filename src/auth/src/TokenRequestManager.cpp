#include "auth/TokenRequestManager.h"

#include <iostream>

#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <spdlog/spdlog.h>

#include "axel/Config.h"
#include "util/http.h"
#include "util/interfaces/IHttpSender.h"
#include "util/path.h"

auth::TokenRequestManager::TokenRequestManager() = default;

namespace http = boost::beast::http;

/// Constructs and sends out the token exchange request. The tokens are then
/// parsed from the response.
/// @param auth_code The authorization code to be included in the token exchange request.
/// @param code_verifier The code verifier to be included in the token exchange request.
/// @param http_sender The HttpSender class that defines the request-sending
/// behaviour and underlying networking.
/// @return The access token
std::string
auth::TokenRequestManager::send_token_request(std::string auth_code,
                                              std::string code_verifier,
                                              std::shared_ptr<util::IHttpSender>
                                              http_sender) const {
    const int MAX_RESPONSE_BODY = 10;

    // Check input validity
    if (auth_code.empty()) {
        throw std::runtime_error("auth_code is empty");
    } else if (code_verifier.empty()) {
        throw std::runtime_error("code_verifier is empty");
    }

#ifdef AXEL_TEST
    std::string host = CONFIG("auth.host.axel.staging");
#else
    std::string host = CONFIG("auth.host.poe");
#endif
    std::string redirect_uri{ "https://" + CONFIG("auth.host.axel.production") +
                              CONFIG("auth.endpoint.redirect") };
    std::string request_body =
    util::make_form_data({ { "client_id", CONFIG("auth.client_id") },
                           { "client_secret", std::getenv("SECRET") },
                           { "grant_type", CONFIG("auth.grant_type") },
                           { "code", std::move(auth_code) },
                           { "redirect_uri", redirect_uri },
                           { "scope",
                             util::concatenate_with_space(
                             { CONFIG("auth.scopes.profile"),
                               CONFIG("auth.scopes.stashes"),
                               CONFIG("auth.scopes.characters"),
                               CONFIG("auth.scopes.item_filter"),
                               CONFIG("auth.scopes.league_accounts") }) },
                           { "code_verifier", std::move(code_verifier) } });

    // Add the headers to the request
    auto full_request =
    util::make_http_request("POST",
                            CONFIG("auth.endpoint.token"),
                            { { "host", host },
                              { "content-type",
                                "application/x-www-form-urlencoded" } },
                            request_body);

    full_request.prepare_payload();  // Automatically set Content-Length

    std::cout << full_request << std::endl;

    auto response_body = http_sender->send_http_request(full_request,
                                                        MAX_RESPONSE_BODY);

    spdlog::info("response: {}", response_body);

    // Extract access token
    boost::json::value json = boost::json::parse(response_body);
    auto access_token = json.at("access_token").as_string().c_str();

    return access_token;
}
