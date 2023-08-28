cmake_minimum_required(VERSION 3.5)

project(BSCALER VERSION 0.9.0 LANGUAGES C CXX)

option(RELEASE "Bscaler release option" OFF)
option(MDL_ONLY "model only option" OFF)

set(BUILD_ENV "CSE" CACHE STRING "Build with that thing")
set_property(CACHE BUILD_ENV PROPERTY STRINGS "CSE" "EYER" "FPGA" "X86")

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if (RELEASE)
  add_compile_options(-DRELEASE)
else()
  add_compile_options(-DMDL_DEBUG)
endif()

if (${BUILD_ENV} STREQUAL "EYER")
  add_compile_options(-DEYER_SIM_ENV)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
elseif (${BUILD_ENV} STREQUAL "CSE")
  add_compile_options(-DCSE_SIM_ENV)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfp64 -mnan=2008 -std=c11 -pthread")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfp64 -mnan=2008 -std=c++11 -pthread")
elseif (${BUILD_ENV} STREQUAL "FPGA")
  add_compile_options(-DFPGA_TEST_ENV)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfp64 -mnan=2008 -std=c11 -pthread ")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfp64 -mnan=2008 -std=c++11 -pthread")
elseif (${BUILD_ENV} STREQUAL "X86")
  add_compile_options(-DMDL_ONLY)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
else()
  message(FATAL_ERROR "Build Compile Environment Failed!")
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/source/3rd_party SRC_3RD_PARTY)

if (${BUILD_ENV} STREQUAL "CSE")
  find_library(NNA_DRIVER libnna.a ${NNA_DRIVER_PATH}/lib)
  include_directories(${NNA_DRIVER_PATH}/include)
  #  add_subdirectory(demo)
else()
  add_subdirectory(tools)
endif()


######################################################
# For Recoder Version
######################################################
# needs to find the generated header file
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/generated
  )
# obtain username
execute_process(
  COMMAND
  whoami
  TIMEOUT
  1
  OUTPUT_VARIABLE
  _user_name
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

# obtain configure time
string(TIMESTAMP _configuration_time "%Y-%m-%d %H:%M:%S")

# in case Git is not available, we default to "unknown"
set(GIT_HASH "unknown")

# find Git and if available set GIT_HASH variable
find_package(Git QUIET)
if(GIT_FOUND)
  execute_process(
	COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h
	OUTPUT_VARIABLE GIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
	ERROR_QUIET
	)
endif()

# generate file version.h based on version.h.in
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
  @ONLY
  )

# rebuild version.hpp every time
add_custom_target(
  get_version
  ALL
  DEPENDS
  ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
  )

#add_subdirectory(test)

add_subdirectory(source)

install(DIRECTORY doc DESTINATION ${CMAKE_INSTALL_PREFIX})
