#include "auth/PkceManager.h"

#include <regex>

#include <gtest/gtest.h>

namespace {
	bool is_base64url(const std::string& url) {
		std::string pattern = "[ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-]{43}";  // URL safe

		std::regex base64url_regex(pattern);

		return std::regex_match(url, base64url_regex);
	}
}  // namespace

class PkceManager_test : public ::testing::Test {
	protected:
		auth::PkceManager pkce_manager;  // Assuming util is the appropriate namespace for PkceManager
};

// Testing function_get_code_verifier
TEST_F(PkceManager_test, get_code_verifier__is_32_bytes_and_url_safe) {
	auto verifier = pkce_manager.get_code_verifier();
	EXPECT_EQ(43, verifier.size());
	EXPECT_TRUE(is_base64url(verifier));
}

// Testing function_get_code_challenge
TEST_F(PkceManager_test, get_code_challenge__is_32_bytes_and_url_safe) {
	auto challenge = pkce_manager.get_code_challenge();
	EXPECT_EQ(43, challenge.size());
	EXPECT_TRUE(is_base64url(challenge));
}
