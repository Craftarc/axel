#ifndef AXEL_AUTH_AUTHCODEMANAGER_H
#define AXEL_AUTH_AUTHCODEMANAGER_H

#include <string>

#include "auth/interfaces/IAuthCodeManager.h"

namespace auth {
    class AuthCodeManager : public IAuthCodeManager {
    public:
        /// @brief Gets the authorization url for the user to visit
        std::string get_auth_url(std::string code_challenge, std::string state_hash) const override;
    };
}

#endif // AXEL_AUTH_AUTHCODEMANAGER_H