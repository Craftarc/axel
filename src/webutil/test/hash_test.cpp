#include <boost/test/unit_test.hpp>
#include <vector>
#include "webutil/hash.h"

BOOST_AUTO_TEST_SUITE(function_generate_secret_bytes)
    
    BOOST_AUTO_TEST_CASE(is_length_32) {
        std::vector<uint8_t> secret_bytes = webutil::generate_secret_bytes();
        BOOST_TEST(secret_bytes.size() == 32);
    }
    
    BOOST_AUTO_TEST_CASE(test_base64_url_encode)
    {
        std::string input = "Hello+World/With/+Padding==";
        std::string expected_output = "Hello-World_With_-Padding";
        
        webutil::base64_url_encode(input);
        BOOST_TEST(input == expected_output);
    }

BOOST_AUTO_TEST_SUITE_END()