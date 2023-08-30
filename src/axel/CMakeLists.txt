# Configure target sources here
set(AXEL_SRC_FILES src/Handler.cpp src/Handler.cpp src/Database.cpp src/DynamoDBClient.cpp src/UpdateManager.cpp src/Item.cpp)
set(AXEL_TESTS_SRC_FILES test/Database_test.cpp)

# axel library
add_library(axel ${AXEL_SRC_FILES})
target_include_directories(axel PUBLIC include ${PROJECT_ROOT})
target_link_libraries(axel PUBLIC auth parse
                      AWS::aws-lambda-runtime)

# axel library tests
add_library(axel_tests OBJECT ${AXEL_TESTS_SRC_FILES})
target_include_directories(axel_tests PUBLIC include test)
target_link_libraries(axel_tests PUBLIC axel axel_mocks GTest::gtest)

# axel library mocks
add_library(axel_mocks INTERFACE)
target_include_directories(axel_mocks INTERFACE include mock)
target_link_libraries(axel_mocks INTERFACE GTest::gmock)
