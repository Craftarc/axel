#include <boost/test/unit_test.hpp>
#include <string>
#include <unordered_map>

#include "webutil/pathutil.h"

BOOST_AUTO_TEST_SUITE(free_pathutil)
    using namespace pathutil;
    
    BOOST_AUTO_TEST_SUITE(function_add_query_parameters)
        
        BOOST_AUTO_TEST_CASE(does_not_change_url_if_no_parameters) {
            std::string url = "http://stackoverflow.com";
            std::unordered_map<std::string, std::string> parameters;
            std::string result = add_query_parameters(url, parameters);
            BOOST_CHECK_EQUAL(result, "http://stackoverflow.com");
        }
        
        BOOST_AUTO_TEST_CASE(handles_one_parameter) {
            std::string url = "http://stackoverflow.com";
            std::unordered_map<std::string, std::string> parameters = {{"foo", "bar"}};
            std::string result = add_query_parameters(url, parameters);
            BOOST_CHECK_EQUAL(result, "http://stackoverflow.com?foo=bar");
        }
        
        BOOST_AUTO_TEST_CASE(can_add_to_existing_query_strings) {
            std::string url = "http://stackoverflow.com?existing=parameter";
            std::unordered_map<std::string, std::string> parameters = {{"foo", "bar"}};
            std::string result = add_query_parameters(url, parameters);
            BOOST_CHECK_EQUAL(result, "http://stackoverflow.com?existing=parameter&foo=bar");
        }
    
    BOOST_AUTO_TEST_SUITE_END()
    
    BOOST_AUTO_TEST_SUITE(function_extract_query_params)
        
        BOOST_AUTO_TEST_CASE(extracts_params) {
            std::string input = "https://www.google.com?param1=value1&param2=value2&param3=value3";
            auto result = pathutil::extract_query_params(input);
            
            BOOST_TEST(result.size() == 3); // Check that 3 parameters are indeed parsed out
            
            // Check parameter values
            BOOST_TEST(result["param1"] == "value1");
            BOOST_TEST(result["param2"] == "value2");
            BOOST_TEST(result["param3"] == "value3");
        }
        
        BOOST_AUTO_TEST_CASE(empty_map_if_empty_params) {
            std::string input;
            auto result = pathutil::extract_query_params(input);
            
            BOOST_TEST(result.empty());
        }
    
    BOOST_AUTO_TEST_SUITE_END()
    
    BOOST_AUTO_TEST_SUITE(function_make_form_data)
        
        BOOST_AUTO_TEST_CASE(adds_pairs_correctly_with_the_right_number_of_delimiters) {
            std::unordered_map<std::string, std::string> params = {{"key1", "value1"},
                                                                   {"key2", "value2"},
                                                                   {"key3", "value3"}};
            
            std::string result = make_form_data(params);
            
            // Check if all key value pairs are present
            BOOST_TEST(result.find("key1=value1") != std::string::npos);
            BOOST_TEST(result.find("key2=value2") != std::string::npos);
            BOOST_TEST(result.find("key3=value3") != std::string::npos);
            
            // Number of '&'s should be (size - 1) = 2
            BOOST_TEST(std::count(result.begin(), result.end(), '&') == params.size() - 1);
        }
        
        BOOST_AUTO_TEST_CASE(handles_empty_params) {
            std::unordered_map<std::string, std::string> params = {};
            std::string result = make_form_data(params);
            
            // Check that the string remains empty
            BOOST_TEST(result.length() == 0);
        }
    
    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
