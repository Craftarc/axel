#include "poe_ninja//get_poe_ninja_data.h"
#include "webutil/HttpSender.h"
#include "webutil/http.h"
#include "config/poe_ninja_config.h"

#include <string>

namespace poe_ninja {
    using namespace webutil;
    
    /// Sends an HTTP GET request to given host and path, retrieve response as a string.
    ///
    /// @param resource_path - Path to specific data resource at host.
    ///
    /// @return - Response data from GET request as string.
    std::string get_item_prices(const std::string& resource_path) {
        auto request = make_http_request("GET", resource_path, {{"host", config::poe_ninja::host}});
        
        return HttpSender().send_http_request(request, 0);
    }
} // poe_ninja
