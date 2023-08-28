cmake_minimum_required(VERSION 3.0)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER "mips-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "mips-linux-gnu-g++")

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall -fPIC")
set(CMAKE_C_FLAGS "-O2 -Wall -fPIC")

project(Aip)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/__release_aip_a1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/jz_aip/)
add_subdirectory(src)
MESSAGE(DEBUG "${CMAKE_C_COMPILER}")
