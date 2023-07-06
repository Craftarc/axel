// TODO: Make this the master test file for this module instead

#define BOOST_TEST_MODULE webutil library tests

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(first_test)
{
    int i = 1;
    BOOST_TEST(i);
    BOOST_TEST(i == 1);
}