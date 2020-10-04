# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run DeviceTest.cpp)

## linking Google_Tests_run with src which will be tested
add_subdirectory("${PROJECT_SOURCE_DIR}/../src" "${PROJECT_BINARY_DIR}/../src")

target_link_libraries(Google_Tests_run  Device)
target_include_directories(Google_Tests_run PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/../src")
target_link_libraries(Google_Tests_run gtest gtest_main)
