cmake_minimum_required (VERSION 3.8)

execute_process (COMMAND ${CMAKE_COMMAND} --version)

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

set (TOP_PROJECT_NAME "cmdm")
project (${TOP_PROJECT_NAME})

#set (BUILD_SHARED_LIBS ON)
set (CMAKE_VERBOSE_MAKEFILE OFF)

if (NOT CMAKE_BIULD_TYPE)
	set (CMAKE_BUILD_TYPE "Release")
endif()

set(CMDM_VERSION_MAJOR 1)
set(CMDM_VERSION_MINOR 0)

execute_process (COMMAND ${CMAKE_CXX_COMPILER} -dumpfullversion -dumpversion
	OUTPUT_VARIABLE GCC_VERSION)

message (STATUS ${GCC_VERSION})

#set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/__release)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/__release)

set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu11 -mfp64")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfp64")

include_directories (./include)

add_subdirectory(src)
