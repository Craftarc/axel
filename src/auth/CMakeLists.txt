# Configure target sources here
set(AUTH_SRC_FILES
    src/OauthManager.cpp src/PkceManager.cpp src/StateHashManager.cpp
    src/TokenRequestManager.cpp src/AuthCodeManager.cpp src/SessionManager.cpp)
set(AUTH_TESTS_SRC_FILES
    test/OauthManager_test.cpp test/PkceManager_test.cpp
    test/StateHashManager_test.cpp test/TokenRequestManager_test.cpp
    test/AuthCodeManager_test.cpp test/SessionManager_test.cpp)

# auth library
add_library(auth ${AUTH_SRC_FILES})
target_include_directories(auth PUBLIC include ${BOTAN_PKG_INCLUDE_DIRS}
                                       {PROJECT_ROOT}})
target_link_libraries(
  auth
  PUBLIC util
         axel
         parse
         test_definitions
         spdlog::spdlog
         Boost::json
         ${BOTAN_PKG_LIBRARIES}
         ${AWSSDK_LINK_LIBRARIES})

# auth library tests
add_library(auth_tests OBJECT ${AUTH_TESTS_SRC_FILES})
target_include_directories(auth_tests PUBLIC test ${PROJECT_ROOT})
target_link_libraries(auth_tests PUBLIC auth auth_mocks util_mocks
                                        axel_mocks GTest::gtest)

# auth library mocks
add_library(auth_mocks INTERFACE)
target_include_directories(auth_mocks INTERFACE include mock)
target_link_libraries(auth_mocks INTERFACE GTest::gmock)
