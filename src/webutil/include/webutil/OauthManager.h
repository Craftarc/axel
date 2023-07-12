#ifndef AXEL_OAUTHMANAGER_H
#define AXEL_OAUTHMANAGER_H

#include <string>

namespace webutil {
    class OauthManager {
    public:
        /**
         * @brief Get the authorization url.
         *
         * The authorization url will contain query parameters specified by the PoE Developer Docs.
         *
         * Link to relevant documentation:
         * https://www.pathofexile.com/developer/docs/authorization
         *
         * @return Authorisation url. This is intended to be served to the user from the Lambda function.
         */
        std::string get_authorization_url();
    
    private:
        // Variables
        std::string authorization_url;
        
        // Methods
        std::string make_authorization_url;
    };
}

#endif //AXEL_OAUTHMANAGER_H
