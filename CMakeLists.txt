cmake_minimum_required(VERSION 3.17)
project(Net)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp)
include(FindPCAP.cmake)
set(PROJECT_HEADERS
        src/device_manager.h
        src/packetio.h
        )

set(PROJECT_SOURCES
        src/device_manager.cpp
        src/packetio.cpp
        )
add_executable(Net main.cpp ${PROJECT_SOURCES})
add_subdirectory(Google_tests)
target_link_libraries(Net PUBLIC ${PCAP_LIBRARY})
