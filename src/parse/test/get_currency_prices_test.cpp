#define BOOST_TEST_MODULE parse_tests

#include "parse/get_currency_prices.h"
#include "poe_ninja_config.h"

#include <boost/test/unit_test.hpp>
#include <boost/json/src.hpp>

namespace {


/**
  * @brief Checks if a given string is a valid JSON
  *
  * Parses the provided string using boost::json::parse
  * and returns a boolean value representing whether
  * the parsing was successful i.e., the string is a
  * valid JSON.
  *
  * @note
  * Empty JSONs i.e. "{}" are still considered valid JSONs.
  *
  * @internal
  *
  * @param json_string The string to be checked if it is a valid JSON.
  * @return True if the string is a valid JSON. False, otherwise.
  */
    bool is_json(const std::string& json_string) {
        boost::json::error_code ec;
        boost::json::parse(json_string, ec);
        return !ec;
    }
}

BOOST_AUTO_TEST_CASE(test_host_constants)
{
    BOOST_TEST(config::poe_ninja::host == "poe.ninja");
}


BOOST_AUTO_TEST_CASE(check_get_currency_json_returns_non_empty_valid_json) {
    using namespace config::poe_ninja::paths;

    // All currency and item endpoints (excluding league)
    std::vector<std::string> paths{
            currency, fragment, divination_card, artifact, oil, incubator, unique_weapon, unique_armour,
            unique_accessory, unique_flask, unique_jewel, skill_gem, cluster_jewel, map, blighted_map,
            blight_ravaged_map, scourged_map, unique_map, delirium_orb, invitation, scarab, base_type, fossil,
            resonator, helmet_enchant, beast, essence, vial
    };

    for (auto& path: paths) {
        path += "&league=" + config::poe_ninja::leagues::crucible;
        std::string output = parse::get_currency_json(config::poe_ninja::host, path);
        boost::json::value json_output = boost::json::parse(output);
        BOOST_REQUIRE(is_json(output)); // First check if it's a valid JSON, if not stop the entire test case

        /* Then check if the JSON is of the correct format. Here it actually only checks if the JSON starts with the key
        "lines", otherwise writing validity checks for all different paths will be quite difficult */
        BOOST_TEST(json_output.as_object().if_contains("lines") != nullptr);
    }
}
