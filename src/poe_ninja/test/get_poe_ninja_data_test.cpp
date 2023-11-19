#include <boost/json.hpp>
#include <gtest/gtest.h>

#include "axel/Config.h"

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
}  // namespace

TEST(constants_test, correct_hostname) {
    EXPECT_EQ(CONFIG("api.host.ninja"), "poe.ninja");
}
