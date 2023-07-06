#include <boost/test/unit_test.hpp>

#include "webutil/pathutil.h"

BOOST_AUTO_TEST_SUITE(function_add_query_parameters)
    using namespace pathutil;

    BOOST_AUTO_TEST_CASE(test_no_parameters) {
        std::string url = "http://stackoverflow.com";
        std::map<std::string, std::string> parameters;
        std::string result = add_query_parameters(url, parameters);
        BOOST_CHECK_EQUAL(result, "http://stackoverflow.com");
    }

    BOOST_AUTO_TEST_CASE(test_one_parameter) {
        std::string url = "http://stackoverflow.com";
        std::map<std::string, std::string> parameters = {{"foo", "bar"}};
        std::string result = add_query_parameters(url, parameters);
        BOOST_CHECK_EQUAL(result, "http://stackoverflow.com?foo=bar");
    }

    BOOST_AUTO_TEST_CASE(test_multiple_parameters) {
        std::string url = "http://stackoverflow.com";
        std::map<std::string, std::string> parameters = {{"foo", "bar"},
                                                         {"baz", "qux"}};
        std::string result = add_query_parameters(url, parameters);
        BOOST_CHECK_EQUAL(result, "http://stackoverflow.com?baz=qux&foo=bar");
    }

    BOOST_AUTO_TEST_CASE(test_existing_query) {
        std::string url = "http://stackoverflow.com?existing=parameter";
        std::map<std::string, std::string> parameters = {{"foo", "bar"}};
        std::string result = add_query_parameters(url, parameters);
        BOOST_CHECK_EQUAL(result, "http://stackoverflow.com?existing=parameter&foo=bar");
    }

BOOST_AUTO_TEST_SUITE_END()
