# Configure target sources here
set(AUTH_SRC_FILES src/OauthManager.cpp src/PkceManager.cpp src/StateHashManager.cpp src/TokenRequestManager.cpp)
set(AUTH_TESTS_SRC_FILES test/auth_tests.cpp test/OauthManager_test.cpp test/PkceManager_test.cpp
    test/StateHashManager_test.cpp test/TokenRequestManager_test.cpp)

# auth library
add_library(auth ${AUTH_SRC_FILES})
target_include_directories(auth PUBLIC include ${BOTAN_PKG_INCLUDE_DIRS} ${PROJECT_ROOT})
target_link_libraries(auth webutil ${BOTAN_PKG_LIBRARIES})


# auth library tests
add_executable(auth_tests ${AUTH_TESTS_SRC_FILES})
target_include_directories(auth_tests PRIVATE include ${PROJECT_ROOT})
target_link_libraries(auth_tests PRIVATE auth webutil Boost::unit_test_framework)