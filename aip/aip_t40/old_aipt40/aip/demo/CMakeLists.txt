### demo ####
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

include_directories(
  ${PROJECT_SOURCE_DIR}/include/3rd_party
  ${PROJECT_SOURCE_DIR}/include/api
  ${PROJECT_SOURCE_DIR}/include/api/hal
)

add_executable(resize_demo
  ${CMAKE_CURRENT_SOURCE_DIR}/resize_demo.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(resize_demo bscaler_api ${NNA_DRIVER})

add_executable(resize_nv12_to_nv12
  ${CMAKE_CURRENT_SOURCE_DIR}/resize_nv12_to_nv12.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(resize_nv12_to_nv12 bscaler_api ${NNA_DRIVER})

add_executable(affine_demo
  ${CMAKE_CURRENT_SOURCE_DIR}/affine_demo.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(affine_demo bscaler_api ${NNA_DRIVER})
 
add_executable(perspective_demo
  ${CMAKE_CURRENT_SOURCE_DIR}/perspective_demo.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(perspective_demo bscaler_api ${NNA_DRIVER})

add_executable(converter_demo
  ${CMAKE_CURRENT_SOURCE_DIR}/converter_demo.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(converter_demo bscaler_api ${NNA_DRIVER})


set(DEMO_FILES
  ${PROJECT_SOURCE_DIR}/demo/affine_demo.cpp
  ${PROJECT_SOURCE_DIR}/demo/converter_demo.cpp
  ${PROJECT_SOURCE_DIR}/demo/resize_demo.cpp
  ${PROJECT_SOURCE_DIR}/demo/perspective_demo.cpp
  ${PROJECT_SOURCE_DIR}/demo/resize_nv12_to_nv12.cpp
  )
install(FILES ${DEMO_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/demo)

set(DEMO_IMAGE_FILES
  ${PROJECT_SOURCE_DIR}/image/sfz.jpeg
  )
