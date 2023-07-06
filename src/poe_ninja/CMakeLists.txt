find_package(Boost
             REQUIRED
             COMPONENTS
             thread
             system
             unit_test_framework)

find_package(OpenSSL REQUIRED)

# poe_ninja library
add_library(poe_ninja src/get_poe_ninja_data.cpp)
target_include_directories(poe_ninja PUBLIC ./include ${PROJECT_ROOT})
target_link_libraries(poe_ninja
                      webutil)


# poe_ninja library tests
add_executable(poe_ninja_tests test/poe_ninja_tests.cpp)
target_include_directories(poe_ninja_tests PRIVATE ./include ${PROJECT_ROOT})
target_link_libraries(poe_ninja_tests
                      Boost::unit_test_framework
                      poe_ninja)
