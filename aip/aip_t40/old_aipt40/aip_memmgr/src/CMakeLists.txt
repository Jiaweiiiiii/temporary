file(GLOB_RECURSE DRIVER_SRCS "*.cc" "*.c")

add_library(drivers_glibc_static STATIC ${DRIVER_SRCS})
add_library(drivers_glibc_shared SHARED ${DRIVER_SRCS})
add_library(drivers_uclibc_static STATIC ${DRIVER_SRCS})
add_library(drivers_uclibc_shared SHARED ${DRIVER_SRCS})

set_target_properties(drivers_glibc_static PROPERTIES
  OUTPUT_NAME "drivers"
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/glibc
  )
set_target_properties(drivers_glibc_shared PROPERTIES
  OUTPUT_NAME "drivers"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/glibc/
  )

set_target_properties(drivers_uclibc_static PROPERTIES
  OUTPUT_NAME "drivers"
  LINK_FLAGS "-muclibc -Wl,--gc-sections"
  COMPILE_FLAGS "-muclibc -DMUCLIBC"
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/uclibc/
  )
set_target_properties(drivers_uclibc_shared PROPERTIES
  OUTPUT_NAME "drivers"
  LINK_FLAGS "-muclibc -Wl,--gc-sections"
  COMPILE_FLAGS "-muclibc -DMUCLIBC"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/uclibc/
  )
install(FILES
  mem_manager/oram_mem.h
  mem_manager/ddr_mem.h
  DESTINATION ./include/mem_manager
  )
install(FILES
  drivers/aie_mmap.h
  drivers/soc_nna.h
  DESTINATION ./include/drivers
  )
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib DESTINATION .)
