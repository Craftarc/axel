#ifndef AXEL_AUTH_MOCKSTATEHASHMANAGER_H
#define AXEL_AUTH_MOCKSTATEHASHMANAGER_H

#include <gmock/gmock.h>
#include "auth/interfaces/IStateHashManager.h"

namespace auth {
    class MockStateHashManager : public IStateHashManager {
    public:
        MOCK_METHOD(std::string, get_state_hash, (), (const, override));
        MOCK_METHOD(bool, check_state_hash, (std::string session_id, std::string state_hash), (override, const));
    };
}
#endif // AXEL_AUTH_MOCKSTATEHASHMANAGER_H