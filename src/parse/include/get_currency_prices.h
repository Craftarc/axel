#ifndef AXEL_SRC_PARSE_INCLUDE_GET_CURRENCY_PRICES_H_
#define AXEL_SRC_PARSE_INCLUDE_GET_CURRENCY_PRICES_H_

#include <string>

namespace parse {
    const std::string poeninja_host = "poe.ninja";
    const std::string poeninja_currency_target = "/api/data/currencyoverview?league=Crucible&type=Currency";

    void get_currency_json();
}
#endif