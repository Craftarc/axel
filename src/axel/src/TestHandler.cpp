#include <spdlog/spdlog.h>

#include "axel/TestHandler.h"
#include "boost/json.hpp"
#include "auth/OauthManager.h"
#include "parse/util.h"

namespace axel {
    invocation_response TestHandler::run(const invocation_request& request) {
        std::string payload = request.payload;
        std::string content_type;
        boost::json::value json = boost::json::parse(payload);
        std::string path = json.at("rawPath").as_string().c_str();
        
        if (path == "/login") {
            auth::OauthManager oauth_manager(config::axel::database::auth, config::axel::database::app);
            
            spdlog::info("TestHandler: Starting authentication session");
            std::string response = oauth_manager.start_auth();
            
            return invocation_response::success(response, "application/json");
        } else if (path == "/") {
            
            auth::OauthManager oauth_manager(config::axel::database::auth, config::axel::database::app);
            std::string query_string = json.at("rawQueryString").as_string().c_str();
            std::string session_id = json.at("headers").at("cookie").as_string().c_str();
            
            spdlog::info("TestHandler: Passing redirect query parameters to OAuthManager");
            oauth_manager.receive_auth(query_string, session_id);
        }
        
        bool success = true;
        
        return {payload, content_type, success};
    }
};
