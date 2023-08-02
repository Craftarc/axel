#include <aws/lambda-runtime/runtime.h>
#include <iostream>
#include <boost/json.hpp>

#include "axel/Handler.h"
#include "auth/OauthManager.h"


using namespace aws::lambda_runtime;
namespace {
    /// @brief Takes a request from the AWS Lambda server runtime and delegates it to the appropriate method to be processed.
    invocation_response handler(const invocation_request& req) {
        std::string payload = req.payload;
        std::string content_type;
        boost::json::value json = boost::json::parse(payload);
        std::string path = json.at("rawPath").as_string().c_str();
        
        if (path == "/login") {
            std::cout << "Entered path: " << path << std::endl;
            
            auth::OauthManager oauth_manager;
            std::string response = oauth_manager.start_auth();
            
            std::cout << "/login flow complete" << std::endl;
            
            return invocation_response::success(response, "application/json");
        } else if (path == "/") {
            std::cout << "Entered path: " << path << std::endl;
            
            auth::OauthManager oauth_manager;
            std::string query_string = json.at("rawQueryString").as_string().c_str();
            std::string session_id = json.at("headers").at("cookie").as_string().c_str();
            
            
            oauth_manager.receive_auth(query_string, session_id);
        }
        
        bool success = true;
        
        return {payload, content_type, success};
    }
}
namespace axel {
    void Handler::run() {
        aws::lambda_runtime::run_handler(handler);
    }
}
