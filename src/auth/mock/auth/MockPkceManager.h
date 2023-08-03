#ifndef AXEL_AUTH_MOCKPKCEMANAGER_H
#define AXEL_AUTH_MOCKPKCEMANAGER_H

#include "auth/interfaces/IPkceManager.h"

namespace auth {
    class MockPkceManager : public IPkceManager {
    public:
        MOCK_METHOD(std::string, get_code_verifier, (), (const, override));
        MOCK_METHOD(std::string, get_code_challenge, (), (const, override));
    };
}

#endif // AXEL_AUTH_MOCKPKCEMANAGER_H