#define BOOST_TEST_MODULE parse_tests

#include "get_currency_prices.h"
#include "poe_ninja_config.h"

#include <boost/test/unit_test.hpp>
#include <boost/json/src.hpp>

namespace {
    // Helper function for json testing
    bool is_json(const std::string& str) {
        boost::json::error_code ec;
        boost::json::parse(str, ec);
        return !ec;
    }
}

BOOST_AUTO_TEST_CASE(test_host_constants)
{
    BOOST_TEST(config::poe_ninja::host == "poe.ninja");
}


BOOST_AUTO_TEST_CASE(check_get_currency_json_returns_non_empty_json) {
    std::vector<std::string> all_paths{
            config::poe_ninja::fossil_path,
            config::poe_ninja::resonator_path,
            config::poe_ninja::essence_path,
            config::poe_ninja::divination_card_path,
            config::poe_ninja::prophecy_path,
            config::poe_ninja::skill_gem_path,
            config::poe_ninja::base_type_path,
            config::poe_ninja::helmet_enchant_path,
            config::poe_ninja::unique_map_path,
            config::poe_ninja::map_path,
            config::poe_ninja::unique_jewel_path,
            config::poe_ninja::unique_accessory,
            config::poe_ninja::beast_path
    };

    for (const auto& path: all_paths) {
        std::string result = parse::get_currency_json(config::poe_ninja::host, path);
        BOOST_CHECK(is_json(result) && !result.empty());
    }
}
