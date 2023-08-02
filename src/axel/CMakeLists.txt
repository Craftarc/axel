# Configure target sources here
set(AXEL_SRC_FILES src/Handler.cpp src/Handler.cpp src/TestHandler.cpp src/Database.cpp)
set(AXEL_TESTS_SRC_FILES test/TestHandler_test.cpp)

configure_file(../auth/test/input/auth-response.json response.txt)

# axel library
add_library(axel ${AXEL_SRC_FILES})
target_include_directories(axel PUBLIC include ${PROJECT_ROOT})
target_link_libraries(axel PUBLIC AWS::aws-lambda-runtime auth parse)

# axel library tests
add_library(axel_tests OBJECT ${AXEL_TESTS_SRC_FILES})
target_include_directories(axel_tests PUBLIC include test)
target_link_libraries(axel_tests PUBLIC axel GTest::Main GTest::GTest ${GMock})
