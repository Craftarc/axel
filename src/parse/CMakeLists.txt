# Configure target sources here
set(PARSE_SRC_FILES src/prices.cpp src/json.cpp src/util.cpp)

set(PARSE_TESTS_SRC_FILES test/util_test.cpp)

# parse library
add_library(parse ${PARSE_SRC_FILES})
target_include_directories(parse PUBLIC include)
target_link_libraries(parse PUBLIC Boost::json)

# parse library tests
add_library(parse_tests OBJECT ${PARSE_TESTS_SRC_FILES})
target_include_directories(parse_tests PUBLIC include)
target_link_libraries(parse_tests PUBLIC parse GTest::gtest)