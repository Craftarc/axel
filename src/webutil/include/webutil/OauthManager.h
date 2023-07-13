#ifndef AXEL_OAUTHMANAGER_H
#define AXEL_OAUTHMANAGER_H

#include <string>
#include <vector>
#include "webutil/PkceManager.h"

namespace webutil {
    /**
     * @brief Responsible for generating providing formatted requests and their required information
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
         * @brief Construct an OauthManager instance.
         *
         * @param pkce_manager Instance of PkceManager.
         */
        OauthManager(const PkceManager& pkce_manager);
        
        /**
         * @brief Get the authorization url.
         *
         * @return Authorisation url. This is intended to be served to the client when it first contacts the Lambda
         * function.
         */
        std::string get_authorization_url();
        
        /**
         * @brief Get the state hash.
         *
         * @return State hash. This is used to track the current authorization
         * request to prevent Cross-Site Request Forgery
         */
        std::string get_state_hash(); // TODO: Maybe no need to expose this?
    
    private:
        std::string authorization_url_;
        const PkceManager& pkce_manager_;
        const std::string state_hash_;
        
        /**
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
