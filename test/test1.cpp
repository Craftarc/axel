#define BOOST_TEST_MODULE MyTestModule

#include <boost/test/unit_test.hpp>

// Example test case
BOOST_AUTO_TEST_CASE(test_addition)
{
    int result = 1 + 1;
    BOOST_TEST(result == 2);
}

// Another test case
BOOST_AUTO_TEST_CASE(test_subtraction)
{
    int result = 5 - 3;
    BOOST_TEST(result == 2);
}


