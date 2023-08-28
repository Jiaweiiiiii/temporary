aux_source_directory (. SRC)

find_package(Threads REQUIRED)

add_library (aip-glibc-static STATIC ${SRC})
set_target_properties(aip-glibc-static PROPERTIES
	OUTPUT_NAME "aip_v20"
	COMPILE_FLAGS "-Wall"
	CLEAN_DIRECT_OUTPUT 1
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/glibc
)

add_library (aip-glibc-shared SHARED ${SRC})
set_target_properties(aip-glibc-shared PROPERTIES
	OUTPUT_NAME "aip_v20"
	COMPILE_FLAGS "-Wall"
	CLEAN_DIRECT_OUTPUT 1
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/glibc
)


add_library (aip-uclibc-static STATIC ${SRC})
set_target_properties(aip-uclibc-static PROPERTIES
	OUTPUT_NAME "aip_v20"
	LINK_FLAGS "-Wl,--gc-sections -muclibc"
	COMPILE_FLAGS "-muclibc -Wall"
	CLEAN_DIRECT_OUTPUT 1
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/uclibc
)

add_library (aip-uclibc-shared SHARED ${SRC})
set_target_properties(aip-uclibc-shared PROPERTIES
	OUTPUT_NAME "aip_v20"
	LINK_FLAGS "-Wl,--gc-sections -muclibc"
	COMPILE_FLAGS "-muclibc -Wall"
	CLEAN_DIRECT_OUTPUT 1
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/uclibc
)



#file(GLOB HEAD_FILES ${CMAKE_SOURCE_DIR}/include/*.h)
set(HEAD_FILES
	${CMAKE_SOURCE_DIR}/include/iaic.h
	${CMAKE_SOURCE_DIR}/include/iaic_core.h
	)
install(FILES ${HEAD_FILES}
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

install(FILES ${CMAKE_SOURCE_DIR}/../kernel/kernel-4.4.94/drivers/video/ingenic_aip_v20/jz_aip_v20_ioctl.h
	DESTINATION ${CMAKE_INSTALL_PREFIX}/include
	RENAME iaic_ioctl.h
)

install(TARGETS aip-glibc-static aip-glibc-shared
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/glibc
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/glibc
)

install(TARGETS aip-uclibc-static aip-uclibc-shared
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/uclibc
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/uclibc
)
