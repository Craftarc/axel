#ifndef AXEL_AUTH_MOCKAUTHCODEMANAGER_H
#define AXEL_AUTH_MOCKAUTHCODEMANAGER_H

#include "auth/interfaces/IAuthCodeManager.h"

namespace auth {
    class MockAuthCodeManager : public IAuthCodeManager {
    public:
        MOCK_METHOD(std::string, get_auth_url, (std::string code_challenge, std::string state_hash), (const, override));
    };
}

#endif // AXEL_AUTH_MOCKAUTHCODEMANAGER_H