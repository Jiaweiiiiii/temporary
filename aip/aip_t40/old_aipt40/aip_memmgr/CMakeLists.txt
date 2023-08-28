cmake_minimum_required(VERSION 3.0)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER "mips-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "mips-linux-gnu-g++")

set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall -fPIC")
set(CMAKE_C_FLAGS "-O2 -Wall -fPIC")

project(Drivers)

option(VENUS_MEM_MSG_ENABLE "set VENUS_MEM_MSG_ENABLE default" OFF)
if(VENUS_MEM_MSG_ENABLE)
  add_definitions(-DVENUS_MEM_MSG)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install_uclibc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(src)
MESSAGE(DEBUG "${CMAKE_C_COMPILER}")
