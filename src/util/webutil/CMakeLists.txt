# Configure target sources here
set(WEBUTIL_SRC_FILES src/http.cpp src/path.cpp src/hash.cpp src/HttpSender.cpp)
set(WEBUTIL_TESTS_SRC_FILES test/http_test.cpp test/path_test.cpp test/hash_test.cpp test/HttpSender_test.cpp)

# webutil library
add_library(webutil ${WEBUTIL_SRC_FILES})
target_include_directories(webutil PUBLIC include test ${BOTAN_PKG_INCLUDE_DIRS} ${PROJECT_ROOT})
target_link_libraries(webutil PUBLIC OpenSSL::SSL OpenSSL::Crypto Boost::thread ${BOTAN_PKG_LIBRARIES})

# webutil library tests
add_library(webutil_tests OBJECT ${WEBUTIL_TESTS_SRC_FILES})
target_include_directories(webutil_tests PUBLIC include)
target_link_libraries(webutil_tests PUBLIC webutil webutil_mocks GTest::gtest)


# webutil mocks
add_library(webutil_mocks INTERFACE)
target_include_directories(webutil_mocks INTERFACE mock include)
target_link_libraries(webutil_mocks INTERFACE GTest::gmock)