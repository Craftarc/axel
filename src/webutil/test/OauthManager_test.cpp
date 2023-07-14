#include <vector>
#include <string>

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include "webutil/OauthManager.h"
#include "webutil/PkceManager.h"
#include "config/poe_auth_config.h"

namespace {
    std::unordered_map<std::string, std::string> extract_query_params(const std::string& url) {
        std::unordered_map<std::string, std::string> params;
        std::string query = url.substr(url.find('?') + 1); // Consider only characters after '?'
        
        std::vector<std::string> query_components;
        boost::split(query_components, query, boost::is_any_of("&"));
        
        for (const auto& component: query_components) {
            std::vector<std::string> key_value_pair;
            boost::split(key_value_pair, component, boost::is_any_of("="));
            if (key_value_pair.size() == 2) {
                params[key_value_pair[0]] = key_value_pair[1];
            }
        }
        return params;
    }
}

BOOST_AUTO_TEST_SUITE(class_OauthManager)
    using namespace webutil;
    
    BOOST_AUTO_TEST_SUITE(function_get_authorization_url)
        
        BOOST_AUTO_TEST_CASE(has_all_required_query_parameters) {
            OauthManager oauth_manager(PkceManager{});
            std::string url = oauth_manager.get_authorization_url();
            auto params = extract_query_params(url);
            
            BOOST_CHECK(params.find("client_id") != params.end());
            BOOST_CHECK(params.find("response_type") != params.end());
            BOOST_CHECK(params.find("scope") != params.end());
            BOOST_CHECK(params.find("state") != params.end());
            BOOST_CHECK(params.find("redirect_uri") != params.end());
            BOOST_CHECK(params.find("code_challenge") != params.end());
            BOOST_CHECK(params.find("code_challenge_method") != params.end());
        }
        
        // Check that query parameters are in line with GGG's requirements
        BOOST_AUTO_TEST_CASE(query_parameters_are_valid) {
            OauthManager oauth_manager(PkceManager{});
            std::string url = oauth_manager.get_authorization_url();
            auto params = extract_query_params(url);
            
            /* Regex for checking if the scope parameter is of the following format:
             * "account:{something}%20account:{something}...account:{something}" */
            boost::regex pattern("(account:\\w+%20)*(account:\\w+)$");
            
            BOOST_CHECK(params["client_id"] == "axel");
            BOOST_CHECK(params["response_type"] == "code");
            BOOST_CHECK(boost::regex_match(params["scope"], pattern));
            BOOST_CHECK(params["state"].size() == 43);
            BOOST_CHECK(
                    params["redirect_uri"] == "https://bibdsedr7muzq2hg5h2rqzs2ke0matoa.lambda-url.us-west-1.on.aws/");
            BOOST_CHECK(params["code_challenge"].size() == 43);
            BOOST_CHECK(params["code_challenge_method"] == "S256");
        }
    
    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()