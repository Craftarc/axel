#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "auth/TokenRequestManager.h"
#include "webutil/MockHttpSender.h"

namespace webutil {
    class TokenRequestManager_test : public ::testing::Test {
    protected:
        auth::TokenRequestManager token_request_manager;
        std::unique_ptr<MockHttpSender> mock_http_sender{std::make_unique<MockHttpSender>()};
        
        // Test vectors
        std::string auth_code{"d25cc653854f1d4db9c0797a30a374c6"};
        std::string code_verifier{"dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk"};
    };
    
    TEST_F(TokenRequestManager_test, send_token_request__extracts_and_returns_tokens) {
        // Dummy response for webutil::send_http_request()
        std::string sample_json = "{\n"
                                  "    \"access_token\": \"486132c90fedb152360bc0e1aa54eea155768eb9\",\n"
                                  "    \"expires_in\": 2592000,\n"
                                  "    \"token_type\": \"bearer\",\n"
                                  "    \"scope\": \"account:profile\",\n"
                                  "    \"username\": \"Novynn\",\n"
                                  "    \"sub\": \"c5b9c286-8d05-47af-be41-67ab10a8c53e\",\n"
                                  "    \"refresh_token\": \"17abaa74e599192f7650a4b89b6e9dfef2ff68cd\"\n"
                                  "}";
        
        EXPECT_CALL(*mock_http_sender,
                    send_http_request(::testing::_)).Times(1).WillOnce(::testing::Return(sample_json));
        
        token_request_manager.send_token_request(auth_code, code_verifier, std::move(mock_http_sender));
    }
}

