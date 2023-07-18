#ifndef AXEL_OAUTHMANAGER_H
#define AXEL_OAUTHMANAGER_H

#include <string>
#include <utility>
#include <vector>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>

#include "PkceManager.h"
#include "StateHashManager.h"
#include "TokenRequestManager.h"

namespace webutil {
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
        
        /// @brief Constructs an OauthManager instance.
        explicit OauthManager(PkceManager pkce_manager, StateHashManager state_hash_manager,
                              TokenRequestManager token_request_manager);
        
        /// @brief Gets the authorization url.
        std::string get_authorization_url();
        
        /// @brief Constucts the HTTP POST request for token exchange
        void make_token_request();
        
        /// @brief Sends the access token exchange request
        void send_token_request(); // TODO: Implement test
        
        
        
        /// @brief Accept an authorization code to be used in future Oauth steps.
        void receive_authorization_code(const std::string& url_string);
        
        /// @brief Retrieves the state hash.
        std::string get_state_hash() const;
    
    private:
        
        /// @brief The states of OauthManager throughout the Oauth process.
        enum class State {
            INITIAL,
            AUTH_SENT,
            AUTH_OK,
            TOKEN_REQUEST_MADE,
            TOKEN_REQUEST_SENT,
            TOKEN_RECEIVED,
            FINISH
        };
        
        /// @brief The current state of OauthManager.
        State state_;
        
        /// @brief Stores the authorization url field to serve to users, complete with query parameters.
        std::string authorization_url_;
        
        /// @brief Stores the authorization code received.
        std::string authorization_code_;
        
        /// @brief Stores the access token exchange request.
        http::request<http::string_body> token_request_;
        
        /// @brief Stores a PkceManager instance.
        ///
        /// There should only be one PkceManager instance per session.
        const PkceManager pkce_manager_;
        
        /// @brief Stores a StateHashManager instance.
        ///
        /// There should only be one StateHashManager instance per session.
        const StateHashManager state_hash_manager_;
        
        /// @brief Stores a TokenRequestManager instance.
        ///
        /// There should only be one TokenRequestManager instance per session.
        TokenRequestManager token_request_manager_;
        
        /// @brief Constucts the authorization url to redirect the users to.
        std::string make_authorization_url();
    };
}

#endif //AXEL_OAUTHMANAGER_H
