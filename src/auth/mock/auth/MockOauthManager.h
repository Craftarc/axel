#ifndef AXEL_MOCKOAUTHMANAGER_H
#define AXEL_MOCKOAUTHMANAGER_H

#include <gmock/gmock.h>
#include "auth/interfaces/IOauthManager.h"

namespace auth {
    class MockOauthManager : public IOauthManager {
        MOCK_METHOD(std::string, start_auth, (), (override));
        MOCK_METHOD(std::string,
                    receive_auth,
                    (const std::string& query_string, const std::string& session_id),
                    (override));
    };
}

#endif //AXEL_MOCKOAUTHMANAGER_H
