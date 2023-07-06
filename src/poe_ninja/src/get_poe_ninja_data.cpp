#include "poe_ninja//get_poe_ninja_data.h"
#include "webutil/https_get.h"
#include "config/poe_ninja_config.h"

#include <string>

/**
 * @brief Fetch data from POE Ninja using specified host and path.
 *
 * Send an HTTP GET request to given host and path, retrieve response as a string.
 *
 * @param resource_path - Path to specific data resource at host.
 *
 * @return - Response data from GET request as string.
 */

namespace poe_ninja {
    std::string get_item_prices(const std::string& resource_path) {
        return webutil::http_get(config::poe_ninja::host, "https", resource_path);
    }
} // poe_ninja
