#include <iostream>
#include "webutil/OauthManager.h"
#include "webutil/pathutil.h"
#include "config/poe_auth_config.h"

namespace webutil {
    std::string OauthManager::get_authorization_url() {
        std::map<std::string, std::string> query_map({{"client_id",     "axel"},
                                                      {"response_type", "code"}});
        
        std::string result = pathutil::add_query_parameters(config::poe::host, config::poe::paths::auth_path,
                                                            query_map);
        std::cout << result << std::endl;
        return result;
        
    }
    
} // webutil