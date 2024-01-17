#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <parse/util.h>

using namespace ::testing;

class util_test : public Test {
    protected:
        int status_code = 200;
        std::unordered_map<std::string, std::string> headers = {
            { "Key 1", "Value 1" },
            { "Key 2", "Value 2" }
        };
        std::string body = "Response body";
};

TEST_F(util_test, make_invocation_response_payload__2_headers) {
    std::string result = parse::make_invocation_response_payload(status_code,
                                                                 headers,
                                                                 body);

    // Allow any ordering of key-value pairs in headers
    std::string regex =
    R"(\{"statusCode":[0-9]{3},"headers":\{("Key 1":"Value 1","Key 2":"Value 2"|"Key 2":"Value 2","Key 1":"Value 1")\},"body":"Response body"\})";

    EXPECT_THAT(result, ContainsRegex(regex));
}

TEST_F(util_test, make_invocation_response_payload__empty_headers) {
    std::unordered_map<std::string, std::string> headers;

    std::string result = parse::make_invocation_response_payload(status_code,
                                                                 headers,
                                                                 body);

    // Expect empty headers object
    std::string regex =
    R"(\{"statusCode":[0-9]{3},"headers":\{\},"body":"Response body"\})";

    EXPECT_THAT(result, ContainsRegex(regex));
}

TEST_F(util_test, make_invocation_response_payload__empty_body) {
    std::string body;

    std::string result = parse::make_invocation_response_payload(status_code,
                                                                 headers,
                                                                 body);

    // Expect empty body string
    std::string regex =
    R"(\{"statusCode":[0-9]{3},"headers":\{("Key 1":"Value 1","Key 2":"Value 2"|"Key 2":"Value 2","Key 1":"Value 1")\},"body":""\})";

    EXPECT_THAT(result, ContainsRegex(regex));
}
