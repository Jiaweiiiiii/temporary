# Install script for directory: /home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/source

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/tmp/to_ATD")
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
  foreach(file
      "$ENV{DESTDIR}/tmp/to_ATD/nna/lib/libnna.so.1.0.0"
      "$ENV{DESTDIR}/tmp/to_ATD/nna/lib/libnna.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/tmp/to_ATD/nna/lib/libnna.so.1.0.0;/tmp/to_ATD/nna/lib/libnna.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/tmp/to_ATD/nna/lib" TYPE SHARED_LIBRARY FILES
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/build/source/libnna.so.1.0.0"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/build/source/libnna.so"
    )
  foreach(file
      "$ENV{DESTDIR}/tmp/to_ATD/nna/lib/libnna.so.1.0.0"
      "$ENV{DESTDIR}/tmp/to_ATD/nna/lib/libnna.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/home/jwzhang/work/aip/custom/gcc720-uclibc09332-r518/bin/mips-linux-uclibc-strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/tmp/to_ATD/nna/lib/libnna.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/tmp/to_ATD/nna/lib" TYPE STATIC_LIBRARY FILES "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/build/source/libnna.a")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/tmp/to_ATD/nna/include/aie_bscaler.h;/tmp/to_ATD/nna/include/aie_mmap.h;/tmp/to_ATD/nna/include/aie_nndma.h;/tmp/to_ATD/nna/include/dma_func.h;/tmp/to_ATD/nna/include/ddr_mem.h;/tmp/to_ATD/nna/include/ivspmon.h;/tmp/to_ATD/nna/include/libxio.h;/tmp/to_ATD/nna/include/macro_def.h;/tmp/to_ATD/nna/include/mxu3.h;/tmp/to_ATD/nna/include/nna.h;/tmp/to_ATD/nna/include/nna_app.h;/tmp/to_ATD/nna/include/nna_dma_driver_v4.h;/tmp/to_ATD/nna/include/nna_regs.h;/tmp/to_ATD/nna/include/nna_dma_memory_v1.h;/tmp/to_ATD/nna/include/nna_reg_set_v.h;/tmp/to_ATD/nna/include/nna_reg_set_c.h;/tmp/to_ATD/nna/include/oram_mem.h;/tmp/to_ATD/nna/include/soc_nna.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/tmp/to_ATD/nna/include" TYPE FILE FILES
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/aie_bscaler.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/aie_mmap.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/aie_nndma.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/dma_func.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/ddr_mem.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/ivspmon.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/libxio.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/macro_def.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/mxu3.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/nna.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/nna_app.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/nna_dma_driver_v4.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/nna_regs.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/nna_dma_memory_v1.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/nna_reg_set_v.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/nna_reg_set_c.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/oram_mem.h"
    "/home/jwzhang/work/aip/aip_t40/old_aipt40/drivers/aie_nna_driver/include/soc_nna.h"
    )
endif()

