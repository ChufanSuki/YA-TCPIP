cmake_minimum_required(VERSION 3.17)
project(Net)

set(CMAKE_CXX_STANDARD 14)
include(FindPCAP.cmake)
add_subdirectory(Google_tests)
add_executable(Net main.cpp src/device.cpp src/device.h src/device_manager.cpp src/device_manager.h)
target_link_libraries(Net ${PCAP_LIBRARY})
