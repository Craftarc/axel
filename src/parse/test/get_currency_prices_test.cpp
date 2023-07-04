#define BOOST_TEST_MODULE parse_tests

#include "get_currency_prices.h"

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_host_constants)
{
    BOOST_TEST(parse::poeninja_host == "poe.ninja");
}

