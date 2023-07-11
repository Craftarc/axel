#ifndef AXEL_SRC_PARSE_INCLUDE_GET_CURRENCY_PRICES_H
#define AXEL_SRC_PARSE_INCLUDE_GET_CURRENCY_PRICES_H

#include <string>

namespace poe_ninja {
/**
 * @brief Fetch data from POE Ninja using specified host and path.
 *
 * Send an HTTP GET request to given host and path, retrieve response as a string.
 *
 * @param resource_path - Path to specific data resource at host.
 *
 * @return - Response data from GET request as string.
 */
    std::string get_item_prices(const std::string& resource_path);
}
#endif