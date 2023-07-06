cmake_minimum_required(VERSION 3.9)
project(axel LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Global properties
set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR})

# AWS Lambda runtime library
find_package(aws-lambda-runtime)

# Boost library
find_package(Boost
             REQUIRED
             COMPONENTS unit_test_framework)

# handler executable
add_executable(handler "handler.cpp") # Runtime handler
target_link_libraries(handler PRIVATE AWS::aws-lambda-runtime)
target_compile_features(handler PRIVATE "cxx_std_17")
target_compile_options(handler PRIVATE "-Wall" "-Wextra")
aws_lambda_package_target(handler) # this line creates a target that packages your binary and zips it up

# main executable
add_executable(main main.cpp)
target_link_libraries(main poe_ninja webutil)
target_compile_options(main PRIVATE "-Wall" "-Wextra")

# Subdirectories
add_subdirectory(./src/poe_ninja)
add_subdirectory(./src/webutil)


