#include <gtest/gtest.h>

#include "webutil/http.h"
#include "config/poe_ninja_config.h"

namespace {
    class http_tests : public ::testing::Test {
    };
}

TEST_F(http_tests, make_http_request__data_is_correct) {
    const std::string http_verb = "POST";
    const std::string path = "/test/path";
    const std::unordered_map<std::string, std::string> fields{{"Content-Type", "application/json"}};
    const std::string body = "Test Body";
    
    auto request = webutil::make_http_request(http_verb, path, fields, body);
    
    // Perform checks
    EXPECT_EQ(http_verb, request.method_string());
    EXPECT_EQ(path, request.target());
    
    // Check that fields have been set correctly
    for (const auto& field: fields) {
        EXPECT_EQ(request[field.first], field.second);
    }
    
    // Check the body
    EXPECT_EQ(body, request.body());
}

