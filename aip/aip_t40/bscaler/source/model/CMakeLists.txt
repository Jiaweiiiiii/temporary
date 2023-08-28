## API ###

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

include_directories(${PROJECT_SOURCE_DIR}/include/api)
include_directories(${PROJECT_SOURCE_DIR}/include/api/hal)
include_directories(${PROJECT_SOURCE_DIR}/include/model)
include_directories(${PROJECT_SOURCE_DIR}/include/model/golden)

aux_source_directory(${PROJECT_SOURCE_DIR}/source/model SRC_MDL)
aux_source_directory(${PROJECT_SOURCE_DIR}/source/model/golden SRC_MDL)
aux_source_directory(${PROJECT_SOURCE_DIR}/source/api SRC_MDL)
list(REMOVE_ITEM SRC_MDL
  "${PROJECT_SOURCE_DIR}/source/api/AutoTime.cpp"
  "${PROJECT_SOURCE_DIR}/source/api/bscaler_api.cpp"
  )

add_library(bscaler_mdl SHARED ${SRC_MDL})
add_library(bscaler_mdl_static STATIC ${SRC_MDL})
 
set_target_properties(bscaler_mdl_static PROPERTIES OUTPUT_NAME "bscaler_mdl")
set_target_properties(bscaler_mdl PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(bscaler_mdl PROPERTIES VERSION ${MAJOR}.${MINOR}.${PATCH})
set_target_properties(bscaler_mdl_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

install(TARGETS bscaler_mdl DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/mdl)
install(TARGETS bscaler_mdl_static DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/mdl)
 
set(MDL_HEAD_FILES
  ${PROJECT_SOURCE_DIR}/include/model/bscaler_mdl.h
  ${PROJECT_SOURCE_DIR}/include/model/bscaler_mdl_api.h
  )
install(FILES ${MDL_HEAD_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

