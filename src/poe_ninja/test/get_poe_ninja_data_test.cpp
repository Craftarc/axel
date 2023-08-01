#include <gtest/gtest.h>
#include <boost/json.hpp>

#include "poe_ninja/get_poe_ninja_data.h"
#include "config/poe_ninja_config.h"

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
  *
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

TEST(constants_test, correct_hostname) {
    EXPECT_EQ(config::poe_ninja::host, "poe.ninja");
}

// PoE Ninja API endpoints test
TEST(constants_test, DISABLED_currency_json_returns_non_empty_valid_json) {
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
        std::string output = poe_ninja::get_item_prices(path);
        
        boost::json::value json_output = boost::json::parse(output);
        
        ASSERT_TRUE(is_json(output));
        
        EXPECT_TRUE(json_output.as_object().if_contains("lines") != nullptr);
    }
}