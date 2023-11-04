#ifndef AXEL_MOCKOAUTHMANAGER_H
#define AXEL_MOCKOAUTHMANAGER_H

#include <gmock/gmock.h>

#include "auth/interfaces/IOauthManager.h"

namespace auth {
    class MockOauthManager : public IOauthManager {
            MOCK_METHOD(void,
                        start_auth,
                        (crow::response& response),
                        (override));
            MOCK_METHOD(void,
                        receive_auth,
                        (const std::string& query_string,
                         const std::string& session_id,
                         crow::response& response),
                        (override));
    };
}  // namespace auth

#endif  //AXEL_MOCKOAUTHMANAGER_H
