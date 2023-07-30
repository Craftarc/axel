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
add_library(poe_ninja_tests OBJECT ${POE_NINJA_TESTS_SRC_FILES})
target_include_directories(poe_ninja_tests PUBLIC include ${PROJECT_ROOT})
target_link_libraries(poe_ninja_tests PUBLIC poe_ninja ${GTEST_BOTH_LIBRARIES})