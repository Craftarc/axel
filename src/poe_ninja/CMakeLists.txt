# Configure target sources here
set(POE_NINJA_SRC_FILES ./src/get_poe_ninja_data.cpp)
set(POE_NINJA_TESTS_SRC_FILES ./test/get_poe_ninja_data_test.cpp)

find_package(Boost REQUIRED COMPONENTS thread system)

find_package(OpenSSL REQUIRED)

# poe_ninja library
add_library(poe_ninja ${POE_NINJA_SRC_FILES})
target_include_directories(poe_ninja PUBLIC ./include ${PROJECT_ROOT})
target_link_libraries(poe_ninja webutil Boost::json)


# poe_ninja library tests
add_executable(poe_ninja_tests ${POE_NINJA_TESTS_SRC_FILES})
target_include_directories(poe_ninja_tests PRIVATE include ${PROJECT_ROOT})
target_link_libraries(poe_ninja_tests PRIVATE poe_ninja ${GTEST_BOTH_LIBRARIES})
include(GoogleTest)
gtest_discover_tests(poe_ninja_tests)