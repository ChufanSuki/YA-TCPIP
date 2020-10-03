cmake_minimum_required(VERSION 3.17)
project(Net)

set(CMAKE_CXX_STANDARD 14)
include(FindPCAP.cmake)
add_executable(Net main.cpp src/device.cpp src/device.h)
target_link_libraries(Net ${PCAP_LIBRARY})
