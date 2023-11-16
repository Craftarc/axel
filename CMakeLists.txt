# Configure target sources here
set(MAIN_SRC_FILES main.cpp)

# Copy python test server into build directory if it changes Python test server
add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/server.py"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/integration/server.py"
          "${CMAKE_BINARY_DIR}/server.py"
  DEPENDS "${CMAKE_SOURCE_DIR}/integration/server.py")

add_custom_target(copy_server ALL DEPENDS "${CMAKE_BINARY_DIR}/server.py")

# Toolchain path
set(CMAKE_TOOLCHAIN_FILE
    /opt/vcpkg/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Path to CMMake toolchain file")
message(STATUS "Toolchain file at: ${CMAKE_TOOLCHAIN_FILE}")

cmake_minimum_required(VERSION 3.22)
project(axel LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -rdynamic -fdiagnostics-color=always")

set(Boost_NO_WARN_NEW_VERSIONS 1)

# Global properties
set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

# Find libraries AWS C++ SDK
find_package(AWSSDK CONFIG REQUIRED COMPONENTS core dynamodb)

# AWS Lambda runtime library
find_package(aws-lambda-runtime)

# Boost library
find_package(Boost REQUIRED COMPONENTS thread system json)

# GTest
find_package(GTest REQUIRED)
enable_testing()

# OpenSSL
find_package(OpenSSL REQUIRED)

# Crow
find_package(Crow REQUIRED)

# Botan uses pkgconfig; find PkgConfig, then use PkgConfig to find botan-2.pc
find_package(PkgConfig REQUIRED)
pkg_check_modules(BOTAN_PKG REQUIRED botan-2)
link_directories(${BOTAN_PKG_LIBRARY_DIRS})

# spdlog
find_package(spdlog CONFIG REQUIRED)

# sqlite3
find_package(unofficial-sqlite3 CONFIG REQUIRED)

# End find libraries

# main executable
add_executable(main ${MAIN_SRC_FILES})
target_include_directories(main PRIVATE AWS::aws-lambda-runtime)
target_link_libraries(main PRIVATE util axel test_definitions Crow::Crow
                                   spdlog::spdlog unofficial::sqlite3::sqlite3)
target_compile_options(main PRIVATE "-Wall" "-Wextra")

# Settings

# Compile-time definitions to link against if the library needs to know whether
# the executable is compiled for test or not
add_library(test_definitions INTERFACE)

option(AXEL_TEST "Use test databases" YES)
if(AXEL_TEST)
  message(STATUS "Using testing databases")
  target_compile_definitions(test_definitions INTERFACE AXEL_TEST=1)
else()
  message(STATUS "Using production databases")
  target_compile_definitions(test_definitions INTERFACE AXEL_TEST=0)
endif()

# End Settings

# sandbox executable (for testing free functions)
add_executable(sandbox sandbox.cpp)
target_compile_options(sandbox PRIVATE "-Wall" "-Wextra")
target_include_directories(sandbox PRIVATE ${PROJECT_ROOT})
target_link_libraries(sandbox PRIVATE util axel unofficial::sqlite3::sqlite3)

# tests executable (all tests)
add_executable(
  tests
  $<TARGET_OBJECTS:auth_tests> $<TARGET_OBJECTS:util_tests>
  $<TARGET_OBJECTS:parse_tests> $<TARGET_OBJECTS:poe_ninja_tests>
  $<TARGET_OBJECTS:axel_tests>)
target_link_libraries(
  tests PRIVATE auth_tests util_tests parse_tests poe_ninja_tests axel_tests
                GTest::gtest_main)

# Subdirectories
add_subdirectory(src/poe_ninja)
add_subdirectory(src/util)
add_subdirectory(src/parse)
add_subdirectory(src/auth)
add_subdirectory(src/axel)
