#ifndef AXEL_UTIL_PARSE_PRICES
#define AXEL_UTIL_PARSE_PRICES

#include <string>
#include <unordered_map>

namespace parse {
    /// @brief Parses the item pricing data from PoE ninja into a table
    std::unordered_map<std::string, double> parse_item_prices(const std::string& item_prices_string);
}

#endif // AXEL_UTIL_PARSE_PRICES