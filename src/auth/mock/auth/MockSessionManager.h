#ifndef AXEL_AUTH_MOCKSESSIONMANAGER_H
#define AXEL_AUTH_MOCKSESSIONMANAGER_H

#include <gmock/gmock.h>

#include "auth/interfaces/ISessionManager.h"

namespace auth {
    class MockSessionManager : public ISessionManager {
    public:
        MOCK_METHOD(std::string, get_session_token, (), (const, override));
    };
}

#endif // AXEL_AUTH_MOCKSESSIONMANAGER_H