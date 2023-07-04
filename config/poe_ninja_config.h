#ifndef AXEL_POE_NINJA_CONFIG_H
#define AXEL_POE_NINJA_CONFIG_H

#include <string>

namespace config {
    const std::string poe_ninja_host = "poe.ninja";
    const std::string poe_ninja_currency_path = "/api/data/currencyoverview?league=Crucible&type=Currency";
    const std::string poe_ninja_fragment_path = "/api/data/currencyoverview?league=Crucible&type=Fragment";
}
#endif //AXEL_POE_NINJA_CONFIG_H
