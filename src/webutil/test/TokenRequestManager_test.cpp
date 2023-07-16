#include <boost/test/unit_test.hpp>
#include <boost/beast/http.hpp>
#include <iostream>

#include "webutil/TokenRequestManager.h"

BOOST_AUTO_TEST_SUITE(class_TokenRequestManager)
    using namespace webutil;
    namespace http = boost::beast::http;
    
    BOOST_AUTO_TEST_SUITE(function_make_token_request)
        
        BOOST_AUTO_TEST_CASE(rejects_empty_auth_code) {
            TokenRequestManager token_request_manager;
            BOOST_CHECK_THROW(token_request_manager.make_token_request("", "verifier"), std::runtime_error);
        }
        
        BOOST_AUTO_TEST_CASE(rejects_empty_verifier) {
            TokenRequestManager token_request_manager;
            BOOST_CHECK_THROW(token_request_manager.make_token_request("code", ""), std::runtime_error);
        }
        
        BOOST_AUTO_TEST_CASE(correct_output_if_valid_input) {
            TokenRequestManager token_request_manager;
            auto result = token_request_manager.make_token_request("code", "verifier").body();
            
            // Check if all required fields are present
            BOOST_TEST(result.find("client_id=axel") != std::string::npos);
            BOOST_TEST(result.find("client_secret=secret") != std::string::npos);
            BOOST_TEST(result.find("grant_type=authorization_code") != std::string::npos);
            BOOST_TEST(result.find("code=code") != std::string::npos);
            BOOST_TEST(
                    result.find("redirect_uri=https://bibdsedr7muzq2hg5h2rqzs2ke0matoa.lambda-url.us-west-1.on.aws/") !=
                    std::string::npos);
            BOOST_TEST(result.find("scope") != std::string::npos);
            BOOST_TEST(result.find("code_verifier=verifier") != std::string::npos);
        }
    
    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()