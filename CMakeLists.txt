# Configure target sources here
set(MAIN_SRC_FILES main.cpp)

cmake_minimum_required(VERSION 3.13)
project(axel LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Global properties
set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

# AWS Lambda runtime library
find_package(aws-lambda-runtime)

# Boost library
find_package(Boost REQUIRED COMPONENTS thread system unit_test_framework json)

# GTest
find_package(GTest REQUIRED)
message(STATUS "Google Test headers found in ${GTEST_INCLUDE_DIRS}")
message(STATUS "Google Test found at ${GTEST_BOTH_LIBRARIES}")

## GMock
# since find_package(GTest) only links in gtest and gtest_main, we need to link in gmock ourselves
find_library(GMock NAMES gmock)
if (NOT GMock)
    message(FATAL_ERROR "GMock not found")
else ()
    message(STATUS "Google Mock found at ${GMock}")
endif ()

# OpenSSL
find_package(OpenSSL REQUIRED)

# Botan uses pkgconfig; find PkgConfig, then use PkgConfig to find botan-2.pc
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(BOTAN_PKG REQUIRED botan-2)

# main executable
add_executable(main ${MAIN_SRC_FILES})
target_include_directories(main PRIVATE AWS::aws-lambda-runtime)
target_link_libraries(main PRIVATE AWS::aws-lambda-runtime Boost::json axel)
target_compile_features(main PRIVATE "cxx_std_17")
target_compile_options(main PRIVATE "-Wall" "-Wextra")
aws_lambda_package_target(main NO_LIBC) # this line creates a target that packages your binary and zips it up

# sandbox executable
add_executable(sandbox sandbox.cpp)
target_compile_features(sandbox PRIVATE "cxx_std_17")
target_compile_options(sandbox PRIVATE "-Wall" "-Wextra")
target_include_directories(sandbox PRIVATE AWS::aws-lambda-runtime)
target_link_libraries(sandbox PRIVATE AWS::aws-lambda-runtime)


# Subdirectories
add_subdirectory(src/poe_ninja)
add_subdirectory(src/webutil)
add_subdirectory(src/auth)
add_subdirectory(src/axel)

