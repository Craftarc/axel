#include <boost/test/unit_test.hpp>

#include "webutil/PkceManager.h"

BOOST_AUTO_TEST_SUITE(class_PkceManager)
    webutil::PkceManager manager;
    
    BOOST_AUTO_TEST_CASE(function_get_code_verifier) {
        BOOST_CHECK_EQUAL(manager.get_code_verifier().length(), 43); // 43 characters from encoding 32 bytes
    }
    
    BOOST_AUTO_TEST_CASE(function_get_code_challenge) {
        // 43 characters from encoding 32 bytes from SHA256
        BOOST_CHECK_EQUAL(manager.get_code_challenge().length(), 43);
    }


BOOST_AUTO_TEST_SUITE_END();