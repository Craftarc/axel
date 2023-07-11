find_package(Boost REQUIRED COMPONENTS thread system unit_test_framework)

find_package(OpenSSL REQUIRED)

# Botan uses pkgconfig; find PkgConfig, then use PkgConfig to find botan-2.pc
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "/path/to/pkgconfig:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(BOTAN_PKG REQUIRED botan-2)


# webutil library
add_library(webutil ./src/https_get.cpp ./src/pathutil.cpp src/PkceManager.cpp)
target_include_directories(webutil PUBLIC ./include ${BOTAN_PKG_INCLUDE_DIRS})
target_link_libraries(webutil OpenSSL::SSL OpenSSL::Crypto Boost::thread ${BOTAN_PKG_LIBRARIES})

# webutil library tests
add_executable(webutil_tests ./test/webutil_tests.cpp)
target_include_directories(webutil_tests PRIVATE ./include ${PROJECT_ROOT})
target_link_libraries(webutil_tests PRIVATE webutil Boost::unit_test_framework)