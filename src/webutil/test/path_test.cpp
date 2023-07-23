#include <gtest/gtest.h>
#include <string>
#include <unordered_map>

#include "webutil/path.h"

class path_tests : public ::testing::Test {
protected:
    std::string url = "http://stackoverflow.com";
    std::string url_with_existing_parameter = "http://stackoverflow.com?existing=parameter";
};

TEST_F(path_tests, add_query_parameters__handles_zero_parameters) {
    std::unordered_map<std::string, std::string> parameters;
    std::string result = webutil::add_query_parameters(url, parameters);
    EXPECT_EQ(result, "http://stackoverflow.com");
}

TEST_F(path_tests, add_query_parameters__adds_one_parameter) {
    std::unordered_map<std::string, std::string> parameters = {{"parse_auth_response", "bar"}};
    std::string result = webutil::add_query_parameters(url, parameters);
    EXPECT_EQ(result, "http://stackoverflow.com?parse_auth_response=bar");
}

TEST_F(path_tests, add_query_parameters__can_add_to_existing_query_strings) {
    std::unordered_map<std::string, std::string> parameters = {{"parse_auth_response", "bar"}};
    std::string result = webutil::add_query_parameters(url_with_existing_parameter, parameters);
    EXPECT_EQ(result, "http://stackoverflow.com?existing=parameter&parse_auth_response=bar");
}

TEST_F(path_tests, extract_query_params__extracts_params) {
    std::string input = "https://www.google.com?param1=value1&param2=value2&param3=value3";
    auto result = webutil::extract_query_params(input);
    
    EXPECT_EQ(result.size(), 3); // Check that 3 parameters are indeed parsed out
    
    // Check parameter values
    EXPECT_EQ(result["param1"], "value1");
    EXPECT_EQ(result["param2"], "value2");
    EXPECT_EQ(result["param3"], "value3");
}

TEST_F(path_tests, extract_query_params__empty_map_if_empty_params) {
    std::string input;
    auto result = webutil::extract_query_params(input);
    
    EXPECT_TRUE(result.empty());
}

TEST_F(path_tests, concatenate_with_space__adds_pairs_correctly_with_the_right_number_of_delimiters) {
    std::unordered_map<std::string, std::string> params = {{"key1", "value1"},
                                                           {"key2", "value2"},
                                                           {"key3", "value3"}};
    
    std::string result = webutil::make_form_data(params);
    
    // Check if all key value pairs are present
    EXPECT_NE(result.find("key1=value1"), std::string::npos);
    EXPECT_NE(result.find("key2=value2"), std::string::npos);
    EXPECT_NE(result.find("key3=value3"), std::string::npos);
    
    // Number of '&'s should be (size - 1) = 2
    EXPECT_EQ(std::count(result.begin(), result.end(), '&'), params.size() - 1);
}

TEST_F(path_tests, concatenate_with_space__handles_empty_params) {
    std::unordered_map<std::string, std::string> params = {};
    std::string result = webutil::make_form_data(params);
    
    // Check that the string remains empty
    EXPECT_EQ(result.length(), 0);
}
