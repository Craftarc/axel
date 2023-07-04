cmake_minimum_required(VERSION 3.9)
project(axel LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(aws-lambda-runtime)
add_executable(handler "handler.cpp") # Runtime handler

target_link_libraries(handler PRIVATE AWS::aws-lambda-runtime)
target_compile_features(handler PRIVATE "cxx_std_17")
target_compile_options(handler PRIVATE "-Wall" "-Wextra")

# this line creates a target that packages your binary and zips it up
aws_lambda_package_target(handler)

add_executable(main main.cpp)
target_link_libraries(main parse)

add_subdirectory(./src/parse)
