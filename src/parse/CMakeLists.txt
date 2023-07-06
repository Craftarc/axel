# Parse library
find_package(Boost
             REQUIRED
             COMPONENTS
             thread
             system)

find_package(OpenSSL REQUIRED)
add_library(parse ./src/get_currency_prices.cpp)
target_include_directories(parse PUBLIC ./include ${PROJECT_ROOT}/config)
target_link_libraries(parse
                      OpenSSL::SSL
                      OpenSSL::Crypto
                      Boost::thread)


# Parse library tests
add_executable(parse_tests ./test/get_currency_prices_test.cpp)
target_include_directories(parse_tests PUBLIC ./include ${PROJECT_ROOT}/config)
target_link_libraries(parse_tests
                      Boost::unit_test_framework
                      parse)
