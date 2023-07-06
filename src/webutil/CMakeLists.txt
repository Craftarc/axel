find_package(Boost REQUIRED COMPONENTS thread system unit_test_framework)

find_package(OpenSSL REQUIRED)


# webutil library
add_library(webutil ./src/https_get.cpp)
target_include_directories(webutil PUBLIC ./include)
target_link_libraries(webutil OpenSSL::SSL OpenSSL::Crypto Boost::thread)

# webutil library tests
add_executable(webutil_tests ./test/https_get_test.cpp)
target_include_directories(webutil_tests PRIVATE ./include)
target_link_libraries(webutil_tests PRIVATE webutil Boost::unit_test_framework)