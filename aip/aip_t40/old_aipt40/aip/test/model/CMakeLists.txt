add_executable(test_resize_api
  ${CMAKE_CURRENT_SOURCE_DIR}/test_resize_api.cpp
  ${SRC_MDL}
  )
target_link_libraries(test_resize_api m bscaler_api pthread)#fixme

add_executable(test_affine_api
  ${CMAKE_CURRENT_SOURCE_DIR}/test_affine_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  ${SRC_MDL}
  )
target_link_libraries(test_affine_api m bscaler_api pthread)#fixme

add_executable(test_perspective_api
  ${CMAKE_CURRENT_SOURCE_DIR}/test_perspective_api.cpp
  ${SRC_MDL}
  )
target_link_libraries(test_perspective_api m bscaler_api pthread)#fixme

add_executable(affine_conformity
  ${CMAKE_CURRENT_SOURCE_DIR}/evaluate/affine_conformity.cpp
  ${SRC_MDL}
  )
target_link_libraries(affine_conformity m bscaler_api pthread)#fixme

add_executable(perspective_accuracy
  ${CMAKE_CURRENT_SOURCE_DIR}/evaluate/perspective_accuracy.cpp
  ${SRC_MDL}
  )
target_link_libraries(perspective_accuracy m bscaler_api pthread)#fixme

add_executable(perspective_conformity
  ${CMAKE_CURRENT_SOURCE_DIR}/evaluate/perspective_conformity.cpp
  ${SRC_MDL}
  )
target_link_libraries(perspective_conformity m bscaler_api pthread)#fixme
