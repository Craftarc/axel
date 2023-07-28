#ifndef AXEL_POE_NINJA_GET_POE_NINJA_DATA_H
#define AXEL_POE_NINJA_GET_POE_NINJA_DATA_H

#include <string>

namespace poe_ninja {
    /// @brief Fetches data from POE Ninja using specified host and path.
    std::string get_item_prices(const std::string& resource_path);
}
#endif // AXEL_POE_NINJA_GET_POE_NINJA_DATA_H