# Install script for directory: /home/jwzhang/work/aip/driver/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/jwzhang/work/aip/driver/7.2.0/2.29")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./include/mem_manager" TYPE FILE FILES
    "/home/jwzhang/work/aip/driver/src/mem_manager/ddr_mem.h"
    "/home/jwzhang/work/aip/driver/src/mem_manager/duplex_list.h"
    "/home/jwzhang/work/aip/driver/src/mem_manager/alloc_manager.h"
    "/home/jwzhang/work/aip/driver/src/mem_manager/LocalMemMgr.h"
    "/home/jwzhang/work/aip/driver/src/mem_manager/buf_list.h"
    "/home/jwzhang/work/aip/driver/src/mem_manager/data.h"
    "/home/jwzhang/work/aip/driver/src/mem_manager/oram.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./include/drivers" TYPE FILE FILES
    "/home/jwzhang/work/aip/driver/src/drivers/aie_mmap.h"
    "/home/jwzhang/work/aip/driver/src/drivers/utils.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/home/jwzhang/work/aip/driver/build/lib")
endif()

