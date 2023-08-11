#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "auth/OauthManager.h"
#include "auth/PkceManager.h"
#include "auth/AuthCodeManager.h"
#include "auth/SessionManager.h"
#include "auth/MockStateHashManager.h"
#include "auth/MockTokenRequestManager.h"
#include "auth/MockOauthManager.h"
#include "auth/MockPkceManager.h"
#include "auth/MockSessionManager.h"
#include "auth/MockAuthCodeManager.h"

#include "axel/MockDatabase.h"

using namespace ::testing;
namespace {
    class OauthManager_test : public Test {
    protected:
        
        std::unique_ptr<auth::MockPkceManager> mock_pkce_manager{std::make_unique<auth::MockPkceManager>()};
        std::unique_ptr<auth::MockStateHashManager> mock_state_hash_manager{std::make_unique<auth::MockStateHashManager>()};
        std::unique_ptr<auth::MockTokenRequestManager> mock_token_request_manager{std::make_unique<auth::MockTokenRequestManager>()};
        std::unique_ptr<auth::MockAuthCodeManager> mock_auth_code_manager{std::make_unique<auth::MockAuthCodeManager>()};
        std::unique_ptr<auth::MockSessionManager> mock_session_manager{std::make_unique<auth::MockSessionManager>()};
        std::unique_ptr<axel::MockDatabase> mock_auth_database{std::make_unique<axel::MockDatabase>()};
        std::unique_ptr<axel::MockDatabase> mock_app_database{std::make_unique<axel::MockDatabase>()};
        
        // Get raw pointers to objects before they are moved into OauthManager()
        auth::MockPkceManager* raw_mock_pkce_manager{mock_pkce_manager.get()};
        auth::MockStateHashManager* raw_mock_state_hash_manager{mock_state_hash_manager.get()};
        auth::MockTokenRequestManager* raw_mock_token_request_manager{mock_token_request_manager.get()};
        auth::MockAuthCodeManager* raw_mock_auth_code_manager{mock_auth_code_manager.get()};
        auth::MockSessionManager* raw_mock_session_manager{mock_session_manager.get()};
        axel::MockDatabase* raw_mock_auth_database{mock_auth_database.get()};
        axel::MockDatabase* raw_mock_app_database{mock_app_database.get()};
        
        auth::OauthManager oauth_manager{
                std::move(mock_pkce_manager),
                std::move(mock_state_hash_manager),
                std::move(mock_token_request_manager),
                std::move(mock_auth_code_manager),
                std::move(mock_session_manager),
                std::move(mock_auth_database),
                std::move(mock_app_database)
        };
    };
}

TEST_F(OauthManager_test, start_auth__calls_expected_functions) {
    // Put session id, state hash and code verifier into database
    EXPECT_CALL(*raw_mock_auth_database, put(_)).Times(1).WillOnce(Return(true));
    
    // Make the authorisation url
    EXPECT_CALL(*raw_mock_auth_code_manager, get_auth_url(_, _)).Times(1).WillOnce(Return("string"));
    
    // Make the call
    EXPECT_NO_THROW(oauth_manager.start_auth());
}

TEST_F(OauthManager_test,
       receive_auth__ok_if_state_hash_matches) { //TODO: Mock oauth_manager.receive_auth() as well because it accesses DynamoDB
    
    // Call the auth database to retrieve session details in a map
    EXPECT_CALL(*raw_mock_auth_database, get(_)).Times(1)
                                                .WillOnce(Return(Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>()));
    
    // State hash matches
    EXPECT_CALL(*raw_mock_state_hash_manager, check_state_hash(_, _)).Times(1).WillOnce(Return(true));
    
    // Make access token request
    EXPECT_CALL(*raw_mock_token_request_manager,
                send_token_request(_, _, _)).Times(1)
                                            .WillOnce(Return("access_token"));
    
    // Get app session token
    EXPECT_CALL(*raw_mock_session_manager, get_session_token()).Times(1).WillOnce(Return("session_token"));
    
    // Store tokens into App Database
    EXPECT_CALL(*raw_mock_app_database, put(_)).Times(1).WillOnce(Return(true));
    
    // Make the call
    EXPECT_NO_THROW(oauth_manager.receive_auth("https://google.com?code=auth_code&state=state_hash",
                                               "session_id",
                                               0));
}

//TODO: Add checks for bad params

TEST_F(OauthManager_test, receive_auth__fail_if_state_hash_does_not_match) {
    // Assume the state hash does not match
    EXPECT_CALL(*raw_mock_state_hash_manager, check_state_hash(_, _)).Times(1).WillOnce(Return(false));
    
    EXPECT_THROW(oauth_manager.receive_auth("https://google.com?code=auth_code&state=state_hash",
                                            "session_id",
                                            0),
                 std::runtime_error);
}