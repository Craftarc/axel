#include "auth/StateHashManager.h"

#include <gtest/gtest.h>
#include <regex>

#include <gtest/gtest.h>

namespace {
    bool is_base64url(const std::string& url) {
        std::string pattern = "[ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstu"
                              "vwxyz0123456789_-]{43}";  // URL safe

        std::regex base64url_regex(pattern);

        return std::regex_match(url, base64url_regex);
    }

}  // namespace

class StateHashManager_test : public ::testing::Test {
    protected:
        auth::StateHashManager state_hash_manager;
        std::string state_hash = state_hash_manager.get_state_hash();
};

TEST_F(StateHashManager_test, get_state_hash__is_43_bytes) {
    EXPECT_EQ(43, state_hash.size());
}

TEST_F(StateHashManager_test, get_state_hash__is_base64url_encoded) {
    EXPECT_TRUE(is_base64url(state_hash));
}
