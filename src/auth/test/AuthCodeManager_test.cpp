#include <gtest/gtest.h>
#include "auth/AuthCodeManager.h"

class AuthCodeManager_test : public ::testing::Test {
protected:
    // Test vectors
    std::string code_challenge{"E9Melhoa2OwvFrEMTJguCHaoeK1t8URWbuGJSstw-cM"};
    std::string state_hash{"10ceb8104963e91e47a95f4138448ecf"};
    
    auth::AuthCodeManager auth_code_manager;
    std::string auth_url{auth_code_manager.get_auth_url(code_challenge, state_hash)};
    std::vector<std::string> required_params{"client_id",
                                             "response_type",
                                             "scope",
                                             "state",
                                             "redirect_uri",
                                             "code_challenge",
                                             "code_challenge_method"};
};

TEST_F(AuthCodeManager_test, get_auth_url__contains_all_required_parameters) {
    
    for (auto const& param: required_params) {
        ASSERT_NE(std::string::npos, auth_url.find(param));
    }
}

