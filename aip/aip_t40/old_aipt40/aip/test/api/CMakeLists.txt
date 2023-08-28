add_executable(rand_resize_api
  ${CMAKE_CURRENT_SOURCE_DIR}/rand_resize_api.cpp
  ${PROJECT_SOURCE_DIR}/tools/random_api/random_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  ${PROJECT_SOURCE_DIR}/source/wrap/bscaler_wrap.c
  )
target_link_libraries(rand_resize_api bscaler_api bscaler_mdl ${NNA_DRIVER} m pthread)

add_executable(rand_affine_api
  ${CMAKE_CURRENT_SOURCE_DIR}/rand_affine_api.cpp
  ${PROJECT_SOURCE_DIR}/tools/random_api/random_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  ${PROJECT_SOURCE_DIR}/source/wrap/bscaler_wrap.c
  )
target_link_libraries(rand_affine_api bscaler_api bscaler_mdl ${NNA_DRIVER} m pthread)

add_executable(rand_perspective_api
  ${CMAKE_CURRENT_SOURCE_DIR}/rand_perspective_api.cpp
  ${PROJECT_SOURCE_DIR}/tools/random_api/random_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  ${PROJECT_SOURCE_DIR}/source/wrap/bscaler_wrap.c
  )
target_link_libraries(rand_perspective_api bscaler_api bscaler_mdl ${NNA_DRIVER} m pthread)

add_executable(read_rand_bin
  ${CMAKE_CURRENT_SOURCE_DIR}/read_rand_bin.cpp
  ${PROJECT_SOURCE_DIR}/tools/random_api/random_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  ${PROJECT_SOURCE_DIR}/source/wrap/bscaler_wrap.c
  )
target_link_libraries(read_rand_bin bscaler_api bscaler_mdl ${NNA_DRIVER} m pthread)

add_executable(rand_converter
  ${CMAKE_CURRENT_SOURCE_DIR}/rand_converter.cpp
  ${PROJECT_SOURCE_DIR}/tools/random_api/random_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  ${PROJECT_SOURCE_DIR}/source/wrap/bscaler_wrap.c
  )
target_link_libraries(rand_converter bscaler_api bscaler_mdl ${NNA_DRIVER} m pthread)

add_executable(bst_and_bsc
  ${CMAKE_CURRENT_SOURCE_DIR}/bst_and_bsc.c
  ${PROJECT_SOURCE_DIR}/tools/random_api/random_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  ${PROJECT_SOURCE_DIR}/source/wrap/bscaler_wrap.c
  )
target_link_libraries(bst_and_bsc bscaler_api bscaler_mdl ${NNA_DRIVER} m pthread)

add_executable(bst_and_bsc1
  ${CMAKE_CURRENT_SOURCE_DIR}/bst_and_bsc1.c
  ${PROJECT_SOURCE_DIR}/tools/random_api/random_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  ${PROJECT_SOURCE_DIR}/source/wrap/bscaler_wrap.c
  )
target_link_libraries(bst_and_bsc1 bscaler_api bscaler_mdl ${NNA_DRIVER} m pthread)

#add_executable(resize_zfni
#  ${CMAKE_CURRENT_SOURCE_DIR}/resize_zfni.cpp
#  ${PROJECT_SOURCE_DIR}/tools/random_api/random_api.cpp
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
#  ${SRC_MDL}
#  )
#target_link_libraries(resize_zfni m bscaler_api pthread)

add_executable(converter
  ${CMAKE_CURRENT_SOURCE_DIR}/converter_demo.cpp
  ${PROJECT_SOURCE_DIR}/tools/random_api/random_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  ${PROJECT_SOURCE_DIR}/source/wrap/bscaler_wrap.c
  )
target_link_libraries(converter bscaler_api ${NNA_DRIVER} m pthread)
