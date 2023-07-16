# Configure target sources here
set(MAIN_SRC_FILES main.cpp)
set(HANDLER_SRC_FILES handler.cpp)

cmake_minimum_required(VERSION 3.9)
project(axel LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Global properties
set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

# AWS Lambda runtime library
find_package(aws-lambda-runtime)

# Boost library
find_package(Boost REQUIRED COMPONENTS thread system unit_test_framework)

# OpenSSL
find_package(OpenSSL REQUIRED)

# Botan uses pkgconfig; find PkgConfig, then use PkgConfig to find botan-2.pc
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(BOTAN_PKG REQUIRED botan-2)

# handler executable
add_executable(handler ${HANDLER_SRC_FILES}) # Runtime handler
target_include_directories(handler PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries(handler PRIVATE AWS::aws-lambda-runtime webutil)
target_compile_features(handler PRIVATE "cxx_std_17")
target_compile_options(handler PRIVATE "-Wall" "-Wextra")
aws_lambda_package_target(handler NO_LIBC) # this line creates a target that packages your binary and zips it up

# main executable
add_executable(main ${MAIN_SRC_FILES})
target_link_libraries(main poe_ninja webutil)
target_compile_options(main PRIVATE "-Wall" "-Wextra")

# Subdirectories
add_subdirectory(src/poe_ninja)
add_subdirectory(src/webutil)
add_subdirectory(src/auth)


