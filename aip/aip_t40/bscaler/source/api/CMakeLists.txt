## API ###

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fvisibility=hidden -Os -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -fvisibility=hidden -Os -fPIC")

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Os -fPIC")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -Os -fPIC")

if (${BUILD_ENV} STREQUAL "EYER")
  include_directories(${PROJECT_SOURCE_DIR}/include/platform/eyer)
endif()
        
include_directories(${PROJECT_SOURCE_DIR}/include/platform)
include_directories(${PROJECT_SOURCE_DIR}/include/api)
include_directories(${PROJECT_SOURCE_DIR}/include/api/hal)

aux_source_directory(${PROJECT_SOURCE_DIR}/source/api/hal SRC_HW_API)
aux_source_directory(${PROJECT_SOURCE_DIR}/source/api SRC_API)
aux_source_directory(${PROJECT_SOURCE_DIR}/source/platform SRC_API)
aux_source_directory(${PROJECT_SOURCE_DIR}/source/wrap SRC_API)

if (RELEASE)
  list(REMOVE_ITEM SRC_API "${PROJECT_SOURCE_DIR}/source/api/AutoTime.cpp")
endif()

#if (RELEASE)
#  add_library(bscaler_api STATIC ${SRC_HW_API} ${SRC_API})
#else()
#  add_library(bscaler_api STATIC ${SRC_HW_API} ${SRC_API} ${SRC_MDL})
#endif()

add_library(bscaler_api SHARED ${SRC_HW_API} ${SRC_API} ${SRC_MDL})
add_library(bscaler_api_static STATIC ${SRC_HW_API} ${SRC_API} ${SRC_MDL})
add_dependencies(bscaler_api get_version)
add_dependencies(bscaler_api_static get_version)

set_target_properties(bscaler_api_static PROPERTIES OUTPUT_NAME "bscaler_api")
set_target_properties(bscaler_api PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(bscaler_api PROPERTIES VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set_target_properties(bscaler_api_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

if (${BUILD_ENV} STREQUAL "EYER")
  target_link_libraries(
	bscaler_api
	${bscaler_api_EXTRA_LDFLAGS}
	"-Wl,--whole-archive"
	driver
	"-Wl,--no-whole-archive"
	"-lpthread"
	)
endif()

install(TARGETS bscaler_api DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/api)
install(TARGETS bscaler_api_static DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/api)
 
set(API_HEAD_FILES
  ${PROJECT_SOURCE_DIR}/include/api/bscaler_api.h
  )
install(FILES ${API_HEAD_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
