#include <gmock/gmock.h>
#include "auth/interfaces/IStateHashManager.h"

namespace auth {
    class MockStateHashManager : public IStateHashManager {
    public:
        MOCK_METHOD(std::string, get_state_hash, (), (const, override));
        MOCK_METHOD(bool, check_state_hash, (const std::string& check_hash), (const, override));
    };
}