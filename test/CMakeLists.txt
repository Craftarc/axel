set(CMAKE_PREFIX_PATH "C:\\msys64\\opt\\boost_1_82_0\\stage")

find_package(Boost
             REQUIRED
             COMPONENTS unit_test_framework)


# 'Boost_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(tests_run test1.cpp)
target_include_directories(tests_run PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(tests_run PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})