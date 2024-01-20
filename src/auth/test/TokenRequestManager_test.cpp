#include "auth/TokenRequestManager.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "util/MockHttpSender.h"

using namespace ::testing;

namespace util {
    class TokenRequestManager_test : public ::testing::Test {
        protected:
            TokenRequestManager_test() :
                auth_code{ "d25cc653854f1d4db9c0797a30a374c6" },
                code_verifier{ "dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk" },
                mock_http_sender{ std::make_shared<MockHttpSender>() } {};

            // Test vectors
            auth::TokenRequestManager token_request_manager;
            std::shared_ptr<MockHttpSender> mock_http_sender;
            std::string code_verifier;
            std::string auth_code;
    };

    TEST_F(TokenRequestManager_test,
           send_token_request__extracts_and_returns_tokens) {
        std::string sample_json =
        "{\n"
        "    \"access_token\": "
        "\"486132c90fedb152360bc0e1aa54eea155768eb9\",\n"
        "    \"expires_in\": 2592000,\n"
        "    \"token_type\": \"bearer\",\n"
        "    \"scope\": \"account:profile\",\n"
        "    \"username\": \"Novynn\",\n"
        "    \"sub\": \"c5b9c286-8d05-47af-be41-67ab10a8c53e\",\n"
        "    \"refresh_token\": "
        "\"17abaa74e599192f7650a4b89b6e9dfef2ff68cd\"\n"
        "}";

        EXPECT_CALL(*mock_http_sender, send_http_request(_, _))
        .Times(1)
        .WillOnce(Return(sample_json));

        auto result =
        token_request_manager.send_token_request(auth_code,
                                                 code_verifier,
                                                 mock_http_sender);
    }
}  // namespace util
