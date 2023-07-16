# Configure target sources here
set(WEBUTIL_SRC_FILES src/httputil.cpp src/pathutil.cpp src/PkceManager.cpp src/OauthManager.cpp src/hash.cpp
    src/StateHashManager.cpp src/TokenRequestManager.cpp)
set(WEBUTIL_TESTS_SRC_FILES test/webutil_tests.cpp test/httputil_test.cpp test/pathutil_test.cpp
    test/PkceManager_test.cpp test/OauthManager_test.cpp test/hash_test.cpp test/StateHashManager_test.cpp test/TokenRequestManager_test.cpp)

find_package(Boost REQUIRED COMPONENTS thread system unit_test_framework)

find_package(OpenSSL REQUIRED)

# Botan uses pkgconfig; find PkgConfig, then use PkgConfig to find botan-2.pc
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(BOTAN_PKG REQUIRED botan-2)


# webutil library
add_library(webutil ${WEBUTIL_SRC_FILES})
target_include_directories(webutil PUBLIC ./include ${BOTAN_PKG_INCLUDE_DIRS} ${PROJECT_ROOT})
target_link_libraries(webutil OpenSSL::SSL OpenSSL::Crypto Boost::thread ${BOTAN_PKG_LIBRARIES})

# webutil library tests
add_executable(webutil_tests ${WEBUTIL_TESTS_SRC_FILES})
target_include_directories(webutil_tests PRIVATE ./include ${PROJECT_ROOT})
target_link_libraries(webutil_tests PRIVATE webutil Boost::unit_test_framework)