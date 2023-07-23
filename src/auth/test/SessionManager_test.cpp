#include <gtest/gtest.h>
#include <regex>
#include "auth/SessionManager.h"

namespace {
    bool is_base64url(const std::string& url) {
        std::string pattern =
                "[ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-]{43}"; // URL safe
        
        std::regex base64url_regex(pattern);
        
        return std::regex_match(url, base64url_regex);
    }
    
    class SessionManager_test : public ::testing::Test {
    
    protected:
        auth::SessionManager session_manager;
        std::string session_token = session_manager.get_session_token();
    };
}

TEST_F(SessionManager_test, get_session_token__is_43_characters) {
    EXPECT_EQ(43, session_token.size());
}

TEST_F(SessionManager_test, get_session_token__is_url_safe) {
    EXPECT_TRUE(is_base64url(session_token));
}

