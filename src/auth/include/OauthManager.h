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

/**
     * @brief Responsible for storing and providing formatted requests and their required information
     * for each step of one Oauth session.
     *
     * Includes methods that serve the authorization URL, state parameter verification and access token exchange.
     * All required messages (URLs, HTTP requests) must not be constructed manually. They are only to be taken from this
     * class.
     *
     * @note Only one OauthManager instance should exist per Oauth session.
     */
    class OauthManager {
    public:
        /**
         * @brief Default constructor
         */
        OauthManager();
        
        /**
         * @brief Construct an OauthManager instance.
         *
         * All Oauth resource (formatted requests, state hashes, etc.) should be obtained through this class only.
         *
         * @param pkce_manager Instance of PkceManager.
         * @param state_hash_manager Instance of StateHashManager.
         * @param token_request_manager Instance of TokenRequestManager.
         *
         */
        explicit OauthManager(PkceManager pkce_manager, StateHashManager state_hash_manager,
                              TokenRequestManager token_request_manager);
        
        /**
         * @brief Get the authorization url.
         *
         * @return Authorisation url. This is intended to be served to the client when it first contacts the Lambda
         * function.
         */
        std::string get_authorization_url();
        
        /**
        * @brief Asks token_request_manager_ to construct the POST request then store it in token_request_
        * Contains all relevant fields as specified in the PoE Developer Docs.
        * Relevant documentation: https://www.pathofexile.com/developer/docs/authorization
        *
        * @note Checks that the authorization code is available in the class and is valid
        * before allowing this operation.
        */
        void make_token_request();
        
        /**
         * @brief Sends the request for exchanging for an access token, then stores the returned
         * access token and refresh token.
         *
         * @note Checks that the access token HTTP request message is ready in the first place
         * before allowing this operation.
         */
        void send_token_request(); // TODO: Implement test
        
        
        /**
        * @brief Accept an authorization code to be used in future Oauth steps.
        *
        * Checks the received HTTP url_string to determined if the state parameter matches, then updates
        * the state of the OauthManager to allow for make_token_request().
        *
        * @param url_string The HTTP url_string containing the authorization code
        * @throws std::runtime_error if state_ is not AUTH_OK, or if the state or authorization code is not found
        * in the query parameters.
        */
        void receive_authorization_code(const std::string& url_string);
        
        /**
         * @brief Retrieves the state hash.
         * @return The state hash string.
         */
        std::string get_state_hash() const;
    
    private:
        enum class State {
            INITIAL,
            AUTH_SENT,
            AUTH_OK,
            TOKEN_REQUEST_MADE,
            TOKEN_REQUEST_SENT,
            TOKEN_RECEIVED,
            FINISH
        };
        
        State state_;
        /**
         * @internal
         * @brief Stores the authorization url field to serve to users, complete with query parameters.
         */
        std::string authorization_url_;
        
        /**
         * @brief Stores the authorization code received
         */
        std::string authorization_code_;
        
        /**
         * @internal
         * @brief Stores the access token exchange request
         */
        http::request<http::string_body> token_request_;
        
        /**
         * @internal
         * @brief Stores a PkceManager instance.
         *
         * There should only be one PkceManager instance per session.
         */
        const PkceManager pkce_manager_;
        
        /**
         * @internal
         * @brief Stores a StateHashManager instance.
         *
         * There should only be one StateHashManager instance per session.
         */
        const StateHashManager state_hash_manager_;
        
        /**
         * @internal
         * @brief Stores a TokenRequestManager instance.
         *
         * There should only be one TokenRequestManager instance per session.
         */
        TokenRequestManager token_request_manager_;
        
        // Methods
        
        /**
         * @internal
         * @brief Constucts the authorization url to redirect the users to.
         *
         * The authorization url will contain query parameters according to the PoE developer documentation's
         * specification.
         *
         * Relevant documentation: https://www.pathofexile.com/developer/docs/authorization
         * @return The complete authorization url
         */
        std::string make_authorization_url();
    };
}

#endif //AXEL_OAUTHMANAGER_H
