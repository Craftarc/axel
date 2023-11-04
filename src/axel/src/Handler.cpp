#include <aws/lambda-runtime/runtime.h>
#include <iostream>
#include <boost/json.hpp>
#include <spdlog/spdlog.h>

#include "axel/Handler.h"
#include "auth/OauthManager.h"

using namespace aws::lambda_runtime;

namespace axel {
    void Handler::run() {
        aws::lambda_runtime::run_handler([this](const invocation_request& req) {
            return this->handler(req);
        });
    }
    
    /// Takes a request from the AWS Lambda server runtime and delegates it to the appropriate method to be processed.
    /// @param req: The invocation request received from the AWS Lambda service
    invocation_response Handler::handler(const invocation_request& req) {
        std::string payload = req.payload;
        spdlog::info("Payload: {}", payload);

        std::string content_type;
        boost::json::value json = boost::json::parse(payload);
        std::string path = json.at("rawPath").as_string().c_str();
        int64_t request_time = json.at("requestContext").at("timeEpoch").as_int64() / 1000; // Milliseconds -> Seconds
        spdlog::debug("Handler::handler: Found request_time '{}' seconds",
                      std::to_string(request_time));
        
        if (path == "/login") {
            spdlog::info("Handler::handler: Entered path '{}'", path);
            std::string response = _oauth_manager.start_auth();
            
            return invocation_response::success(response, "application/json");
        } else if (path == "/") {
            spdlog::info("Handler::handler: Entered path '{}'", path);
            
            std::string query_string = json.at("rawQueryString").as_string().c_str();
            spdlog::debug("Handler::handler: Found query_string '{}'", query_string);
            
            std::string session_id = json.at("headers").at("cookie").as_string().c_str();
            spdlog::debug("Handler::handler: Found session_id '{}'", session_id);
            
            _oauth_manager.receive_auth(query_string, session_id);
            return invocation_response::success("", "application/json");
        }
        
        bool success = true;
        
        return {payload, content_type, success};
    }
    
    /// For example, selecting "local" would mean that database queries are made to testing databases instead of live ones.
    /// @param environment The environment this handler runs in. Can be either "production" or "local".
    Handler::Handler(const std::string& environment) : _oauth_manager(init(environment)) {};
}
