#include "auth/OauthManager.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "auth/AuthCodeManager.h"
#include "auth/MockAuthCodeManager.h"
#include "auth/MockOauthManager.h"
#include "auth/MockPkceManager.h"
#include "auth/MockSessionManager.h"
#include "auth/MockStateHashManager.h"
#include "auth/MockTokenRequestManager.h"
#include "auth/PkceManager.h"
#include "auth/SessionManager.h"
#include "util/MockDatabase.h"

using namespace ::testing;

namespace {
    class OauthManager_test : public Test {
        protected:
            OauthManager_test() {}

            std::unique_ptr<auth::MockPkceManager> mock_pkce_manager{
                std::make_unique<auth::MockPkceManager>()
            };
            std::unique_ptr<auth::MockStateHashManager> mock_state_hash_manager{
                std::make_unique<auth::MockStateHashManager>()
            };
            std::unique_ptr<auth::MockTokenRequestManager>
            mock_token_request_manager{
                std::make_unique<auth::MockTokenRequestManager>()
            };
            std::unique_ptr<auth::MockAuthCodeManager> mock_auth_code_manager{
                std::make_unique<auth::MockAuthCodeManager>()
            };
            std::unique_ptr<auth::MockSessionManager> mock_session_manager{
                std::make_unique<auth::MockSessionManager>()
            };
            std::unique_ptr<util::MockDatabase> mock_database{
                std::make_unique<util::MockDatabase>()
            };

            // Get raw pointers to objects before they are moved into OauthManager()
            auth::MockPkceManager* raw_mock_pkce_manager{
                mock_pkce_manager.get()
            };
            auth::MockStateHashManager* raw_mock_state_hash_manager{
                mock_state_hash_manager.get()
            };
            auth::MockTokenRequestManager* raw_mock_token_request_manager{
                mock_token_request_manager.get()
            };
            auth::MockAuthCodeManager* raw_mock_auth_code_manager{
                mock_auth_code_manager.get()
            };
            auth::MockSessionManager* raw_mock_session_manager{
                mock_session_manager.get()
            };
            util::MockDatabase* raw_mock_database{ mock_database.get() };

            auth::OauthManager oauth_manager{
                std::move(mock_pkce_manager),
                std::move(mock_state_hash_manager),
                std::move(mock_token_request_manager),
                std::move(mock_auth_code_manager),
                std::move(mock_session_manager),
                std::move(mock_database)
            };
    };
}  // namespace

