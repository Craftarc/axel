#include <boost/test/unit_test.hpp>

#include "webutil/StateHashManager.h"

BOOST_AUTO_TEST_SUITE(class_StateHashManager)
    using namespace webutil;
    BOOST_AUTO_TEST_SUITE(function_get_state_hash)
        
        BOOST_AUTO_TEST_CASE(has_43_characters) {
            StateHashManager state_hash_manager;
            BOOST_CHECK_EQUAL(state_hash_manager.get_state_hash().length(), 43); // 32 byte --base64url--> 43 characters
        }
    
    BOOST_AUTO_TEST_SUITE_END()
    
    BOOST_AUTO_TEST_SUITE(function_verify_state_hash)
        
        BOOST_AUTO_TEST_CASE(accepts_matching_hash) {
            StateHashManager state_hash_manager;
            auto state_hash = state_hash_manager.get_state_hash();
            BOOST_CHECK(state_hash_manager.verify_state_hash(state_hash));
        }
        
        BOOST_AUTO_TEST_CASE(rejects_different_hash) {
            StateHashManager state_hash_manager;
            std::string bad_hash = "abc";
            BOOST_CHECK(!state_hash_manager.verify_state_hash(bad_hash));
        }
    
    BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE_END()