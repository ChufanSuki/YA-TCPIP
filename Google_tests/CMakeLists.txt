# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run DeviceTest.cpp)

## linking Google_Tests_run with src which will be tested
target_link_libraries(Google_Tests_run )

target_link_libraries(Google_Tests_run gtest gtest_main)
