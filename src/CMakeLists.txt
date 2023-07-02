find_package(Boost
             REQUIRED
             COMPONENTS thread)

add_executable(http_client http_client.cpp)

target_include_directories(http_client PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(http_client Boost::thread)

