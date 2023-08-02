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
            auth::OauthManager oauth_manager;
            std::string response = oauth_manager.start_auth();
            std::cout << response << std::endl;
        } else if (path == "/") {
            auth::OauthManager oauth_manager;
            std::string query_string = json.at("rawQueryString").as_string().c_str();
            std::string session_id = json.at("headers").at("cookie").as_string().c_str();
            
            std::cout << "Session id: " << session_id << std::endl;
            
            oauth_manager.receive_auth(query_string, session_id);
        }
        
        bool success = true;
        
        return {payload, content_type, success};
    }
};
