#include <boost/json.hpp>
#include "parse/prices.h"

namespace json = boost::json;
namespace parse {
    
    std::unordered_map<std::string, double> parse_item_prices(const std::string& item_prices_string) {
        // Parse into JSON value
        json::value item_prices = json::parse(item_prices_string);
        
        return {};
    }
} // parse