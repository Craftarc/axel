add_library(parse ./src/get_currency_prices.cpp)
target_include_directories(parse PUBLIC ./include /usr/include/openssl)
target_link_libraries(parse pthread ssl crypto)