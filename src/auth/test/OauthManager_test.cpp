#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "auth/OauthManager.h"
#include "auth/PkceManager.h"
#include "auth/AuthCodeManager.h"
#include "auth/SessionManager.h"

#include "mock/MockStateHashManager.h"
#include "mock/MockTokenRequestManager.h"

using namespace ::testing;
namespace {
    class OauthManager_test : public Test {
    protected:
        std::unique_ptr<auth::MockStateHashManager> mock_state_hash_manager{std::make_unique<auth::MockStateHashManager>()};
        std::unique_ptr<auth::MockTokenRequestManager> mock_token_request_manager{std::make_unique<auth::MockTokenRequestManager>()};
        
        // Get raw pointers to objects before they are moved in OauthManager()
        auth::MockStateHashManager* raw_mock_state_hash_manager{mock_state_hash_manager.get()};
        auth::MockTokenRequestManager* raw_mock_token_request_manager{mock_token_request_manager.get()};
        
        auth::OauthManager oauth_manager{
                std::make_unique<auth::PkceManager>(),
                std::move(mock_state_hash_manager),
                std::move(mock_token_request_manager),
                std::make_unique<auth::AuthCodeManager>(),
                std::make_unique<auth::SessionManager>()
        };
    };
}

TEST_F(OauthManager_test, receive_auth__ok_if_state_hash_matches) {
    // Assume the state hash matches
    EXPECT_CALL(*raw_mock_state_hash_manager, check_state_hash(_)).Times(1).WillOnce(Return(true));
    
    // Return a dummy pair of tokens instead of making an actual request
    EXPECT_CALL(*raw_mock_token_request_manager,
                send_token_request(_, _, _)).Times(1)
                                            .WillOnce(Return(std::make_pair<std::string, std::string>("access_token",
                                                                                                      "refresh_token")));
    
    // Make the call
    EXPECT_NO_THROW(oauth_manager.receive_auth("https://google.com?code=auth_code&state=state_hash"));
}

TEST_F(OauthManager_test, receive_auth__fail_if_state_hash_does_not_match) {
    // Assume the state hash does not match
    EXPECT_CALL(*raw_mock_state_hash_manager, check_state_hash(_)).Times(1).WillOnce(Return(false));
    
    EXPECT_THROW(oauth_manager.receive_auth("https://google.com?code=auth_code&state=state_hash"), std::runtime_error);
}