#ifndef AXEL_SRC_PARSE_INCLUDE_GET_CURRENCY_PRICES_H
#define AXEL_SRC_PARSE_INCLUDE_GET_CURRENCY_PRICES_H

#include <string>

namespace poe_ninja {
    std::string get_item_prices(const std::string& resource_path);
}
#endif