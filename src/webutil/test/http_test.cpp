#include <boost/test/unit_test.hpp>
#include <boost/asio/ssl/error.hpp>

#include "webutil/http.h"
#include "config/poe_ninja_config.h"

BOOST_AUTO_TEST_SUITE(free_httputil)
    using namespace webutil;
    namespace http = boost::beast::http;
    
    BOOST_AUTO_TEST_SUITE(function_make_http_request)
        
        BOOST_AUTO_TEST_CASE(TestMakeHttpRequest)
        {
            const std::string http_verb = "POST";
            const std::string path = "/test/path";
            const std::unordered_map<std::string, std::string> fields{{"Content-Type", "application/json"}};
            const std::string body = "Test Body";
            
            http::request<http::string_body> request
                    = make_http_request(http_verb, path, fields, body);
            
            // Perform checks
            BOOST_TEST(http_verb == request.method_string());
            BOOST_TEST(path == request.target());
            
            // Check that fields have been set correctly
            for (const auto& field: fields) {
                BOOST_TEST(request[field.first] == field.second);
            }
            
            // Check the body
            BOOST_TEST(body == request.body());
        }
    
    BOOST_AUTO_TEST_SUITE_END()
    
    BOOST_AUTO_TEST_SUITE(function_send_http_request)
        
        BOOST_AUTO_TEST_CASE(accepts_empty_path) {
            auto request = make_http_request("GET", "", {{"host", "www.google.com"}}, "");
            
            BOOST_CHECK_NO_THROW(webutil::send_http_request(request));
        }
        
        BOOST_AUTO_TEST_CASE(accepts_root_path) {
            auto request = make_http_request("GET", "/", {{"host", "www.google.com"}}, "");
            
            BOOST_CHECK_NO_THROW(webutil::send_http_request(request));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()