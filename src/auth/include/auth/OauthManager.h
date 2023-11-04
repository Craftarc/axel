#ifndef AXEL_AUTH_OAUTHMANAGER_H
#define AXEL_AUTH_OAUTHMANAGER_H

#include <string>
#include <utility>
#include <vector>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>

#include "auth/AuthCodeManager.h"
#include "auth/PkceManager.h"
#include "auth/SessionManager.h"
#include "auth/interfaces/IOauthManager.h"
#include "auth/interfaces/IStateHashManager.h"
#include "auth/interfaces/ITokenRequestManager.h"
#include "crow.h"
#include "util/Database.h"
#include "util/interfaces/IDatabase.h"

namespace auth {

    /// @brief Responsible for storing and providing formatted requests
    /// and their required information
    /// for each step of one Oauth session.
    /// Includes methods that serve the authorization URL, state
    /// parameter verification and access token exchange.
    /// All required messages (URLs, HTTP requests) must not be constructed
    /// manually. They are only to be taken from this
    /// @note Only one OauthManager instance should exist per Oauth session.
    class OauthManager : public IOauthManager {
        public:
            OauthManager();

            /// @brief Default constructor
            OauthManager(std::unique_ptr<IPkceManager> pkce_manager,
                         std::unique_ptr<IStateHashManager> state_hash_manager,
                         std::unique_ptr<ITokenRequestManager>
                         token_request_manager,
                         std::unique_ptr<IAuthCodeManager> auth_code_manager,
                         std::unique_ptr<ISessionManager> session_manager,
                         std::unique_ptr<util::IDatabase> database);

            /// @brief Starts the Oauth process
            void start_auth(crow::response& response) override;

            /// @brief Receives the authorization query string and auth session
            /// id, makes the token exchange request,
            /// and returns the app session id
            void receive_auth(const std::string& query_string,
                              const std::string& session_id,
                              crow::response& response) override;

        private:
            std::unique_ptr<IPkceManager> pkce_manager_;
            std::unique_ptr<IAuthCodeManager> auth_code_manager_;
            std::unique_ptr<IStateHashManager> state_hash_manager_;
            std::unique_ptr<ITokenRequestManager> token_request_manager_;
            std::unique_ptr<ISessionManager> session_manager_;
            std::unique_ptr<util::IDatabase> database_;
    };
}  // namespace auth

#endif  //AXEL_AUTH_OAUTHMANAGER_H
