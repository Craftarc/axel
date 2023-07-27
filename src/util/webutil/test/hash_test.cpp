#include <gtest/gtest.h>
#include <vector>
#include "webutil/hash.h"

class hash_tests : public ::testing::Test {
};

TEST(hash_tests, generate_secret_bytes__is_length_32) {
    std::vector<uint8_t> secret_bytes = webutil::generate_secret_bytes();
    EXPECT_EQ(secret_bytes.size(), 32u);
}

TEST(hash_tests, base64_url_encode__converts_padding) {
    std::string input = "Hello+World/With/+Padding==";
    std::string expected_output = "Hello-World_With_-Padding";
    
    webutil::base64_url_encode(input);
    EXPECT_EQ(input, expected_output);
}