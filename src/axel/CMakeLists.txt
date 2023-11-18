# Configure target sources here
set(AXEL_SRC_FILES
    src/Database.cpp
    src/DynamoDBClient.cpp
    src/Item.cpp
    src/ResourceManager.cpp
    src/PlayerItems.cpp
    src/Exception.cpp
    src/Prices.cpp
    src/Config.cpp)

set(AXEL_TESTS_SRC_FILES test/Database_test.cpp test/ResourceManager_test.cpp
                         test/PlayerItems_test.cpp test/Prices_test.cpp)

# Find libbfd for axel::Exception
find_library(BFD_LIBRARY NAMES bfd REQUIRED)

# axel library
add_library(axel ${AXEL_SRC_FILES})
message(STATUS "${Boost_INCLUDE_DIRS}")
target_include_directories(axel PUBLIC include ${PROJECT_ROOT}
                                       ${Boost_INCLUDE_DIRS})
target_link_libraries(axel PUBLIC auth parse poe_ninja util AWS::aws-lambda-runtime
    ${BFD_LIBRARY})

# axel library tests
add_library(axel_tests OBJECT ${AXEL_TESTS_SRC_FILES})
target_include_directories(axel_tests PUBLIC include test)
target_link_libraries(axel_tests PUBLIC axel axel_mocks util_mocks GTest::gtest)

# axel library mocks
add_library(axel_mocks INTERFACE)
target_include_directories(axel_mocks INTERFACE mock)
target_link_libraries(axel_mocks INTERFACE GTest::gmock)
