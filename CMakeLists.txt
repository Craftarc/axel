cmake_minimum_required(VERSION 3.9)
project(demo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
project(demo LANGUAGES CXX)
find_package(aws-lambda-runtime)
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE AWS::aws-lambda-runtime)
target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_11")
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra")

# this line creates a target that packages your binary and zips it up
aws_lambda_package_target(${PROJECT_NAME})

#add_executable(HttpClient src/http_client.cpp) #TODO: Move this into separate cmakelists in src
#
## Find and set global library properties
#set(CMAKE_PREFIX_PATH "C:\\msys64\\opt\\boost_1_82_0")
#
## Will stop if the library is not found
#find_package(Boost
#        REQUIRED)
#
#
#target_include_directories(HttpClient PUBLIC ${Boost_INCLUDE_DIRS})
#target_link_libraries(HttpClient ws2_32)
#
#add_subdirectory(test)
