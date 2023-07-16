#include "poe_ninja//get_poe_ninja_data.h"
#include "webutil/httputil.h"
#include "config/poe_ninja_config.h"

#include <string>

namespace poe_ninja {
    using namespace webutil;
    
    std::string get_item_prices(const std::string& resource_path) {
        auto request = make_http_request("GET", resource_path, {{"host", config::poe_ninja::host}});
        
        return webutil::send_http_request(request);
    }
} // poe_ninja
