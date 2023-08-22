#ifndef AXEL_AUTH_OAUTHMANAGER_H
#define AXEL_AUTH_OAUTHMANAGER_H

#include <string>
#include <utility>
#include <vector>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>

#include "auth/PkceManager.h"
#include "auth/interfaces/IStateHashManager.h"
#include "auth/interfaces/ITokenRequestManager.h"
#include "auth/AuthCodeManager.h"
#include "auth/SessionManager.h"
#include "axel/Database.h"
#include "auth/interfaces/IOauthManager.h"
#include "axel/interface/IDatabase.h"

namespace auth {
    
    /// @brief Responsible for storing and providing formatted requests and their required information
    /// for each step of one Oauth session.
    ///
    /// Includes methods that serve the authorization URL, state parameter verification and access token exchange.
    /// All required messages (URLs, HTTP requests) must not be constructed manually. They are only to be taken from this
    /// class.
    ///
    /// @note Only one OauthManager instance should exist per Oauth session.
    class OauthManager : public IOauthManager {
    public:
        /// @brief Default constructor, intialises *Manager members with defaults.
        OauthManager(const std::string& auth_database, const std::string& app_database);
        /// @brief Default constructor
        OauthManager(std::unique_ptr<IPkceManager> pkce_manager,
                     std::unique_ptr<IStateHashManager> state_hash_manager,
                     std::unique_ptr<ITokenRequestManager> token_request_manager,
                     std::unique_ptr<IAuthCodeManager> auth_code_manager,
                     std::unique_ptr<ISessionManager> session_manager,
                     std::unique_ptr<axel::IDatabase> auth_database,
                     std::unique_ptr<axel::IDatabase> app_database);
        /// @brief Starts the Oauth process
        std::string start_auth() override;
        /// @brief Receives the authorization query string and auth session id, makes the token exchange request,
        /// and returns the app session id
        std::string receive_auth(const std::string& query_string, const std::string& session_id) override;
    
    private:
        /// @brief The states of OauthManager throughout the Oauth process.
        enum class State {
            INITIAL,
            USER_REDIRECTED,
            AUTH_RECEIVED,
            TOKENS_RECEIVED,
            SESSION_ESTABLISHED,
            ERROR
        };
        
        std::unique_ptr<IPkceManager> pkce_manager_;
        std::unique_ptr<IAuthCodeManager> auth_code_manager_;
        std::unique_ptr<IStateHashManager> state_hash_manager_;
        std::unique_ptr<ITokenRequestManager> token_request_manager_;
        std::unique_ptr<ISessionManager> session_manager_;
        std::unique_ptr<axel::IDatabase> auth_database_;
        std::unique_ptr<axel::IDatabase> app_database_;
        State state_;
        
        void set_state(State state);
    };
}

#endif //AXEL_AUTH_OAUTHMANAGER_H
