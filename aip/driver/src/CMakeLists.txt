file(GLOB_RECURSE DRIVER_SRCS "*.cc" "*.c")

add_library(drivers_glibc_static STATIC ${DRIVER_SRCS})
add_library(drivers_glibc_shared SHARED ${DRIVER_SRCS})
add_library(drivers_uclibc_static STATIC ${DRIVER_SRCS})
add_library(drivers_uclibc_shared SHARED ${DRIVER_SRCS})

set_target_properties(drivers_glibc_static PROPERTIES
  OUTPUT_NAME ${OUT_LIB_NAME}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/glibc
  )
set_target_properties(drivers_glibc_shared PROPERTIES
  OUTPUT_NAME ${OUT_LIB_NAME}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/glibc/
  )

set_target_properties(drivers_uclibc_static PROPERTIES
  OUTPUT_NAME ${OUT_LIB_NAME}
  LINK_FLAGS "-muclibc -Wl,--gc-sections"
  COMPILE_FLAGS "-muclibc -DMUCLIBC"
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/uclibc/
  )
set_target_properties(drivers_uclibc_shared PROPERTIES
  OUTPUT_NAME ${OUT_LIB_NAME}
  LINK_FLAGS "-muclibc -Wl,--gc-sections"
  COMPILE_FLAGS "-muclibc -DMUCLIBC"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/uclibc/
  )
install(FILES
  mem_manager/ddr_mem.h
  mem_manager/duplex_list.h
  mem_manager/alloc_manager.h
  mem_manager/LocalMemMgr.h
  mem_manager/buf_list.h
  mem_manager/data.h
  mem_manager/oram.h
  DESTINATION ./include/mem_manager
  )
install(FILES
  drivers/aie_mmap.h
  drivers/utils.h
  DESTINATION ./include/drivers
  )
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib DESTINATION .)
