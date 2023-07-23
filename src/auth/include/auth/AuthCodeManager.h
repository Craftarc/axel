#ifndef AXEL_AUTH_AUTHCODEMANAGER_H
#define AXEL_AUTH_AUTHCODEMANAGER_H

#include <string>

namespace auth {
    class AuthCodeManager {
    public:
        /// @brief Gets the authorization url for the user to visit
        std::string get_auth_url(std::string code_challenge, std::string state_hash) const;
    };
}

#endif // AXEL_AUTH_AUTHCODEMANAGER_H