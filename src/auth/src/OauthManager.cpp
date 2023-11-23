#include "auth/OauthManager.h"

#include <iostream>
#include <utility>

#include <aws/core/client/ClientConfiguration.h>
#include <botan/base64.h>
#include <spdlog/spdlog.h>

#include "auth/PkceManager.h"
#include "auth/SessionManager.h"
#include "auth/StateHashManager.h"
#include "auth/TokenRequestManager.h"
#include "auth/interfaces/IStateHashManager.h"
#include "auth/interfaces/ITokenRequestManager.h"
#include "axel/Exception.h"
#include "util/HttpSender.h"
#include "util/hash.h"
#include "util/path.h"
#include "util/type.h"

namespace {
    int MAX_AUTH_CODE_TIME =
    30;  // Maximum time before authorisation code expires
         // and can no longer be exchanged
    int MAX_AUTH_START_TIME =
    30;  // Maximum time between first start_auth() and redirect response

    /// @brief Returns the Unix time in seconds
    int64_t get_time_now() {
        std::chrono::time_point time_now{ std::chrono::system_clock::now() };

        // Get duration member
        auto time_point_duration{ time_now.time_since_epoch() };

        // Cast to seconds
        auto duration_seconds{ std::chrono::duration_cast<std::chrono::seconds>(
        time_point_duration) };

        // Extract count member, the number of seconds
        return duration_seconds.count();
    }
}  // namespace

namespace auth {
    OauthManager::OauthManager(const std::string& database) :
        OauthManager{ std::make_unique<PkceManager>(),
                      std::make_unique<StateHashManager>(),
                      std::make_unique<TokenRequestManager>(),
                      std::make_unique<AuthCodeManager>(),
                      std::make_unique<SessionManager>(),
                      std::make_unique<util::Database>(database.c_str()) } {}

    OauthManager::OauthManager(
    std::unique_ptr<IPkceManager> pkce_manager,
    std::unique_ptr<IStateHashManager> state_hash_manager,
    std::unique_ptr<ITokenRequestManager> token_request_manager,
    std::unique_ptr<IAuthCodeManager> auth_code_manager,
    std::unique_ptr<ISessionManager> session_manager,
    std::unique_ptr<util::IDatabase> database) :
        pkce_manager_(std::move(pkce_manager)),
        state_hash_manager_(std::move(state_hash_manager)),
        token_request_manager_(std::move(token_request_manager)),
        auth_code_manager_(std::move(auth_code_manager)),
        session_manager_(std::move(session_manager)),
        database_(std::move(database)) {}

    /// Responsible for calling functions and methods in sequence according
    /// to the Oauth specification.
    void OauthManager::start_auth(crow::response& response) {
        int64_t time_to_live{ get_time_now() + MAX_AUTH_CODE_TIME };
        spdlog::info("time to live set to '{}'", std::to_string(time_to_live));

        auto code_challenge = pkce_manager_->get_code_challenge();
        auto code_verifier = pkce_manager_->get_code_verifier();
        auto state_hash = state_hash_manager_->get_state_hash();
        auto session_id = session_manager_->get_session_token();

        database::OAuthTable oauth_table_row{
            session_id,
            state_hash,
            code_challenge,
            code_verifier,
            static_cast<int>(time_to_live)
        };  // Narrowing

        database_->insert_row("oauth", oauth_table_row);

        std::string auth_url = auth_code_manager_->get_auth_url(code_challenge,
                                                                state_hash);
        spdlog::debug("Authorisation URL: {}", auth_url);

        // Set up response
        response.code = 302;  // Redirect
        response.set_header("Location", auth_url);
        std::string cookie{
            "session_id=" + session_id +
            "; SameSite=Strict; Secure; HttpOnly; Path=/auth"  // Only for auth
                                                               // purposes
        };
        response.set_header("Set-Cookie", cookie);
        return;
    }

    /// Processes the authorization response from the authorization server
    /// after the user has logged in and approved
    /// the required scopes.
    /// @param query_string The query_string string containing the query
    /// parameters "state" and "code", returned from
    /// the authorization server after a successful login by the user.
    /// @param session_id The session_id identifying the authentication request.
    /// @return The session token for tracking this user session.
    void auth::OauthManager::receive_auth(const std::string& query_string,
                                          const std::string& session_id,
                                          crow::response& response) {
        int64_t time_to_live{ get_time_now() + MAX_AUTH_CODE_TIME };

        // Get important query params
        std::unordered_map query_params =
        util::extract_query_params(query_string);
        auto state_hash = query_params["state"];
        auto auth_code = query_params["code"];

        // Keys are not found in map
        if (state_hash.empty() || auth_code.empty()) {
            response.code = 403;
            spdlog::error(
            "State or authorisation code could not be found in query string");
            return;
        }

        // Retrieve information about the Oauth session identified by session_id
        std::vector<std::string> attributes{ "session_id",
                                             "state_hash",
                                             "code_verifier" };
        std::unordered_map item_map = database_->select_row("oauth",
                                                            session_id,
                                                            attributes);

        // Get the stored state hash and code verifier out
        std::string stored_hash{};
        std::string code_verifier{};
        if (!item_map.empty()) {
            stored_hash = util::get_string(item_map.at("state_hash"));
            code_verifier = util::get_string(item_map.at("code_verifier"));
        }

        // Validate state hash
        if (state_hash_manager_->check_state_hash(state_hash, stored_hash)) {
            spdlog::info("State hash is valid");
        } else {
            // Stop immediately if state hash returned from auth server does not match
            spdlog::error(
            "Stored state hash '{0}' does not match input state hash '{1}'",
            stored_hash,
            state_hash);
            response.code = 403;  // Forbidden
            return;
        }

#if AXEL_TEST
        // Just make a fake one
        auto random_bytes = util::generate_secret_bytes();
        auto access_token = Botan::base64_encode(random_bytes.data(),
                                                 random_bytes.size());
        util::base64_url_encode(access_token);
#else
        std::string access_token =
        token_request_manager_
        ->send_token_request(auth_code,
                             code_verifier,
                             std::make_shared<util::HttpSender>());
#endif

        std::string axel_session_id = session_manager_->get_session_token();

        // Store session details in oauth_app
        database::AppTable apptable_row{ axel_session_id,
                                         access_token,
                                         static_cast<int>(time_to_live) };

        database_->insert_row("app", apptable_row);

        // Set up response
        response.code = 200;  // Redirect

        // Cookie and its attributes
        std::string cookie{
            "axel_session_id=" + axel_session_id +
            "; SameSite=Strict; Secure; HttpOnly; Path=/"  // Accessible everywhere
        };
        response.set_header("Set-Cookie", cookie);
        return;
    }
}  // namespace auth
