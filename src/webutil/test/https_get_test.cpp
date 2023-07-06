#include <boost/test/unit_test.hpp>
#include <boost/asio/ssl/error.hpp>

#include "webutil/https_get.h"
#include "config/poe_ninja_config.h"

BOOST_AUTO_TEST_SUITE(function_http_get)

    BOOST_AUTO_TEST_CASE(accepts_empty_path) {
        BOOST_CHECK_NO_THROW(webutil::https_get("www.example.com", "https", ""));
    }

    BOOST_AUTO_TEST_CASE(accepts_root_path) {
        BOOST_CHECK_NO_THROW(webutil::https_get("www.example.com", "https", "/"));
    }

    BOOST_AUTO_TEST_CASE(cannot_use_https) {
        BOOST_CHECK_THROW(webutil::https_get("www.example.com", "http", "/"), std::exception);
        // HACK: Should use a narrow exception instead of std::exception, which is too general
    }

BOOST_AUTO_TEST_SUITE_END()