#ifndef AXEL_OAUTHMANAGER_H
#define AXEL_OAUTHMANAGER_H

#include <string>
#include <utility>
#include <vector>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>

#include "auth/PkceManager.h"
#include "auth/StateHashManager.h"
#include "auth/TokenRequestManager.h"
#include "auth/AuthCodeManager.h"
#include "auth/SessionManager.h"

namespace auth {
    namespace http = boost::beast::http;
    
    /// @brief Responsible for storing and providing formatted requests and their required information
    /// for each step of one Oauth session.
    ///
    /// Includes methods that serve the authorization URL, state parameter verification and access token exchange.
    /// All required messages (URLs, HTTP requests) must not be constructed manually. They are only to be taken from this
    /// class.
    ///
    /// @note Only one OauthManager instance should exist per Oauth session.
    class OauthManager {
    public:
        /// @brief Default constructor
        OauthManager();
        
        /// @brief Starts the Oauth process.
        std::string start_auth();
        std::string receive_auth(const std::string& url);
        
        /// @brief The states of OauthManager throughout the Oauth process.
        enum class State {
            INITIAL,
            USER_REDIRECTED,
            AUTH_CODE_RECEIVED,
            TOKENS_RECEIVED,
            SESSION_ESTABLISHED,
            ERROR
        };
    private:
        const PkceManager pkce_manager_;
        const AuthCodeManager auth_code_manager_;
        const StateHashManager state_hash_manager_;
        const TokenRequestManager token_request_manager_;
        const SessionManager session_manager_;
        State state_;
        
        void set_state(State state);
    };
}

#endif //AXEL_OAUTHMANAGER_H
