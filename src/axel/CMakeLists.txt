# Configure target sources here
set(AXEL_SRC_FILES src/Handler.cpp src/Handler.cpp src/TestHandler.cpp)
set(AXEL_TESTS_SRC_FILES test/TestHandler_test.cpp)
set(AXEL_TEST_INPUT_FILES axel/response.json)

configure_file(test/input/response.json response.txt)

# axel library
add_library(axel ${AXEL_SRC_FILES})
target_include_directories(axel PUBLIC include)
target_link_libraries(axel PUBLIC AWS::aws-lambda-runtime)

# axel library tests
add_executable(axel_tests ${AXEL_TESTS_SRC_FILES})
target_include_directories(axel_tests PRIVATE include test)
target_link_libraries(axel_tests PRIVATE axel GTest::Main GTest::GTest ${GMock})
include(GoogleTest)
gtest_discover_tests(axel_tests)

# Sample AWS Lambda requests for testing
configure_file(test/input/response.json ${CMAKE_BINARY_DIR}/test_input/${AXEL_TEST_INPUT_FILES})