# Parse library
add_library(parse ./src/get_currency_prices.cpp)
target_include_directories(parse PUBLIC ./include ../../config)
target_link_libraries(parse pthread ssl crypto)


# Parse library tests
add_executable(parse_tests ./test/get_currency_prices_test.cpp)
target_include_directories(parse_tests PUBLIC ./include)
target_link_libraries(parse_tests Boost::unit_test_framework)