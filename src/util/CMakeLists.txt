# Configure target sources here
set(UTIL_SRC_FILES src/http.cpp src/path.cpp src/hash.cpp src/HttpSender.cpp src/type.cpp)
set(UTIL_TESTS_SRC_FILES test/http_test.cpp test/path_test.cpp test/hash_test.cpp test/HttpSender_test.cpp)

# util library
add_library(util ${UTIL_SRC_FILES})
target_include_directories(util PUBLIC include test ${BOTAN_PKG_INCLUDE_DIRS} ${PROJECT_ROOT})
target_link_libraries(util PUBLIC OpenSSL::SSL OpenSSL::Crypto Boost::thread ${BOTAN_PKG_LIBRARIES})

# util library tests
add_library(util_tests OBJECT ${UTIL_TESTS_SRC_FILES})
target_include_directories(util_tests PUBLIC include)
target_link_libraries(util_tests PUBLIC util util_mocks GTest::gtest)


# util mocks
add_library(util_mocks INTERFACE)
target_include_directories(util_mocks INTERFACE mock include)
target_link_libraries(util_mocks INTERFACE GTest::gmock)