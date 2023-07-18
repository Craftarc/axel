#ifndef AXEL_SRC_PARSE_INCLUDE_GET_CURRENCY_PRICES_H
#define AXEL_SRC_PARSE_INCLUDE_GET_CURRENCY_PRICES_H

#include <string>

namespace poe_ninja {
    /// @brief Fetches data from POE Ninja using specified host and path.
    std::string get_item_prices(const std::string& resource_path);
}
#endif