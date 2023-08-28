file(GLOB_RECURSE AIP_SRCS "*.cc" "*.c")

add_library(drivers_glibc_static STATIC ${AIP_SRCS})
add_library(drivers_glibc_shared SHARED ${AIP_SRCS})
add_library(drivers_uclibc_static STATIC ${AIP_SRCS})
add_library(drivers_uclibc_shared SHARED ${AIP_SRCS})

set_target_properties(drivers_glibc_static PROPERTIES
  OUTPUT_NAME "aip"
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/glibc
  )
set_target_properties(drivers_glibc_shared PROPERTIES
  OUTPUT_NAME "aip"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/glibc/
  )

set_target_properties(drivers_uclibc_static PROPERTIES
  OUTPUT_NAME "aip"
  LINK_FLAGS "-muclibc -Wl,--gc-sections"
  COMPILE_FLAGS "-muclibc -DMUCLIBC"
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/uclibc/
  )
set_target_properties(drivers_uclibc_shared PROPERTIES
  OUTPUT_NAME "aip"
  LINK_FLAGS "-muclibc -Wl,--gc-sections"
  COMPILE_FLAGS "-muclibc -DMUCLIBC"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/uclibc/
  )
install(FILES
  ../include/jz_aip/ingenic_aip.h
  DESTINATION ./include/
  )
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib DESTINATION .)
