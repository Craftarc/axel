cmake_minimum_required(VERSION 3.21)
project(Backend)

add_executable(HttpClient src/http_client.cpp) #TODO: Move this into separate cmakelists in src

# Find and set global library properties
set(CMAKE_PREFIX_PATH "C:\\msys64\\opt\\boost_1_82_0")

# Will stop if the library is not found
find_package(Boost
             REQUIRED)


target_include_directories(HttpClient PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(HttpClient ws2_32)

add_subdirectory(test)
