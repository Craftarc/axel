#include <aws/lambda-runtime/runtime.h>
#include <iostream>
#include <boost/json.hpp>

#include "axel/Handler.h"
#include "auth/OauthManager.h"

using namespace aws::lambda_runtime;

namespace axel {
    void Handler::run() {
        aws::lambda_runtime::run_handler([this](const invocation_request& req) {
            return this->handler(req);
        });
    }
    
    /// @brief Takes a request from the AWS Lambda server runtime and delegates it to the appropriate method to be processed.
    invocation_response Handler::handler(const invocation_request& req) {
        std::string payload = req.payload;
        std::cout << payload << std::endl;
        std::string content_type;
        boost::json::value json = boost::json::parse(payload);
        std::string path = json.at("rawPath").as_string().c_str();
        
        if (path == "/login") {
            std::cout << "Entered path: " << path << std::endl;
            
            std::string response = _oauth_manager.start_auth();
            
            std::cout << "/login flow complete" << std::endl;
            
            return invocation_response::success(response, "application/json");
        } else if (path == "/") {
            std::cout << "Entered path: " << path << std::endl;
            
            std::string query_string = json.at("rawQueryString").as_string().c_str();
            
            std::string session_id = json.at("headers").at("cookie").as_string().c_str();
            _oauth_manager.receive_auth(query_string, session_id);
            return invocation_response::success("", "application/json");
        }
        
        bool success = true;
        
        return {payload, content_type, success};
    }
    
    /// Sets up the handler with the correct environment variables and configurations. For example, selecting "local"
    /// would mean that database queries are made to testing databases instead of live ones.
    /// @param environment The environment this handler runs in. Can be either "production" or "local"
    Handler::Handler(const std::string& environment) : _oauth_manager(init(environment)) {};
    
    auth::OauthManager Handler::init(const std::string& environment) {
        if (environment == "production") {
            return {config::axel::database::auth, config::axel::database::app};
        } else if (environment == "test") {
            return {config::axel::database::auth_test, config::axel::database::app_test};
        } else {
            throw std::runtime_error("Unknown environment specified for Handler");
        }
    }
}
