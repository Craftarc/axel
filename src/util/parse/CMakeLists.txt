# Configure target sources here
set(PARSE_SRC_FILES src/string.cpp)
set(PARSE_TESTS_SRC_FILES test/string_test.cpp)

# parse library
add_library(parse ${PARSE_SRC_FILES})
target_include_directories(parse PUBLIC include)

# parse library tests
add_library(parse_tests OBJECT ${PARSE_TESTS_SRC_FILES})
target_include_directories(parse_tests PRIVATE include)
target_link_libraries(parse_tests PRIVATE parse ${GTEST_BOTH_LIBRARIES} pthread)