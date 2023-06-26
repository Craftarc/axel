cmake_minimum_required(VERSION 3.21)
project(Backend)
add_executable(HttpClient src/http_client.cpp)

set(CMAKE_PREFIX_PATH "C:\\msys64\\opt\\boost_1_82_0")

find_package(Boost
             REQUIRED)

if (Boost_FOUND)
    message(${Boost_INCLUDE_DIR})
endif ()


target_include_directories(HttpClient PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(HttpClient ws2_32)




