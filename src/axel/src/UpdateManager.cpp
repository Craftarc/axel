#include <boost/beast/http.hpp>

#include "axel/UpdateManager.h"
#include "webutil/http.h"
#include "config/poe.h"

namespace axel {
    UpdateManager::UpdateManager(const std::string& access_token) : _access_token{access_token} {}
    
    http::request<http::string_body> UpdateManager::make_stash_request() const {
        const std::unordered_map<std::string, std::string> fields{{"authorization", "Bearer " + _access_token},
                                                                  {"host", config::poe::api}};
        return webutil::make_http_request("GET", "/stash/ancestor", fields);
    }
    
    /// Gets information about a player's stash for a particular softcore league
    /// @param league: Softcore league name to get items for
    boost::json::value UpdateManager::get_update() const {
        
        return boost::json::value();
    }
}