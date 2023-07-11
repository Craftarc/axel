#include "poe_ninja//get_poe_ninja_data.h"
#include "webutil/https_get.h"
#include "config/poe_ninja_config.h"

#include <string>


namespace poe_ninja {
    std::string get_item_prices(const std::string& resource_path) {
        return webutil::https_get(config::poe_ninja::host, "https", resource_path);
    }
} // poe_ninja
