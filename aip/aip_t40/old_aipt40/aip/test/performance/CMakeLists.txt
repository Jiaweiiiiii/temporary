### demo ####
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

include_directories(
  ${PROJECT_SOURCE_DIR}/include/3rd_party
  ${PROJECT_SOURCE_DIR}/include/api
  ${PROJECT_SOURCE_DIR}/include/api/hal
)

add_executable(resize_perform
  ${CMAKE_CURRENT_SOURCE_DIR}/resize_perform.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(resize_perform bscaler_api ${NNA_DRIVER})

add_executable(resize_nv2nv_perform
  ${CMAKE_CURRENT_SOURCE_DIR}/resize_nv2nv_perform.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(resize_nv2nv_perform bscaler_api ${NNA_DRIVER})

add_executable(resize_nv2bgr_perform
  ${CMAKE_CURRENT_SOURCE_DIR}/resize_nv2bgr_perform.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(resize_nv2bgr_perform bscaler_api ${NNA_DRIVER})

add_executable(converter_perform
  ${CMAKE_CURRENT_SOURCE_DIR}/converter_perform.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(converter_perform bscaler_api ${NNA_DRIVER})

add_executable(affine_perform
  ${CMAKE_CURRENT_SOURCE_DIR}/affine_perform.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(affine_perform bscaler_api ${NNA_DRIVER})

add_executable(perspective_perform
  ${CMAKE_CURRENT_SOURCE_DIR}/perspective_perform.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(perspective_perform bscaler_api ${NNA_DRIVER})

add_executable(threads_test
  ${CMAKE_CURRENT_SOURCE_DIR}/threads_test.cpp
  ${SRC_3RD_PARTY}
  )
target_link_libraries(threads_test bscaler_api ${NNA_DRIVER})

set(DEMO_IMAGE_FILES
  ${PROJECT_SOURCE_DIR}/image/sfz.jpeg
  )
