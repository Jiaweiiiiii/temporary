set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

#find package openmp for mutli-progress
if (NOT EYER)
  find_package(OpenMP QUIET)
  if (OPENMP_FOUND)
    message(STATUS "OPENMP Found")
    add_compile_options(-DOPENMP_EN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  endif()
endif()

include_directories(
  ${PROJECT_SOURCE_DIR}/test
  ${PROJECT_SOURCE_DIR}/include/3rd_party
  ${PROJECT_SOURCE_DIR}/include/model/golden
  ${PROJECT_SOURCE_DIR}/include/hw_api
  ${PROJECT_SOURCE_DIR}/api_random
)

if (CSE)
  include_directories(
    ${PROJECT_SOURCE_DIR}/driver/cse
  )
else()
  include_directories(
    ${PROJECT_SOURCE_DIR}/driver/eyer
  )
endif()


aux_source_directory(${PROJECT_SOURCE_DIR}/source/model/golden SRC_MDL)

################ test ##################
add_executable(perspective_accuracy
  ${CMAKE_CURRENT_SOURCE_DIR}/evaluate/perspective_accuracy.cpp
  ${SRC_MDL}
  )

if (NOT MDL_ONLY)
  target_link_libraries(perspective_accuracy m pthread bscaler_api driver)
else()
  target_link_libraries(perspective_accuracy m pthread bscaler_api)
endif()
 
add_executable(perspective_conformity
  ${CMAKE_CURRENT_SOURCE_DIR}/evaluate/perspective_conformity.cpp
  ${SRC_MDL}
  )
if (NOT MDL_ONLY)
  target_link_libraries(perspective_conformity m pthread bscaler_api driver)
else()
  target_link_libraries(perspective_conformity m pthread bscaler_api)
endif()
 
add_executable(affine_conformity
  ${CMAKE_CURRENT_SOURCE_DIR}/evaluate/affine_conformity.cpp
  ${SRC_MDL}
  )
if (NOT MDL_ONLY)
  target_link_libraries(affine_conformity m pthread bscaler_api driver)
else()
  target_link_libraries(affine_conformity m pthread bscaler_api)
endif()

add_executable(t_bscaler_c
  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_c.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bscaler_random.c
  ${PROJECT_SOURCE_DIR}/source/model/bscaler_mdl.c
  )
if (NOT MDL_ONLY)
  target_link_libraries(t_bscaler_c m pthread bscaler_api driver)
else()
  target_link_libraries(t_bscaler_c m pthread bscaler_api)
endif()

add_executable(t_bscaler_chain
  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_chain_c.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bscaler_random.c
  ${PROJECT_SOURCE_DIR}/source/model/bscaler_mdl.c
  )
if (NOT MDL_ONLY)
  target_link_libraries(t_bscaler_chain m pthread bscaler_api driver)
else()
  target_link_libraries(t_bscaler_chain m pthread bscaler_api)
endif()

add_executable(t_bscaler_t
  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_t.c
  ${PROJECT_SOURCE_DIR}/source/model/bscaler_mdl.c
  )
if (NOT MDL_ONLY)
  target_link_libraries(t_bscaler_t m pthread bscaler_api driver)
else()
  target_link_libraries(t_bscaler_t m pthread bscaler_api)
endif()

add_executable(t_lzma
  ${CMAKE_CURRENT_SOURCE_DIR}/t_lzma.c
  )
if (NOT MDL_ONLY)
  target_link_libraries(t_lzma m pthread bscaler_api driver)
else()
  target_link_libraries(t_lzma m pthread bscaler_api)
endif()

add_executable(rand_test_api_affine
  ${CMAKE_CURRENT_SOURCE_DIR}/api_random/rand_test_affine_api.cpp
  ${SRC_MDL}
  )
if (NOT MDL_ONLY)
  target_link_libraries(rand_test_api_affine m pthread bscaler_api driver)
else()
  target_link_libraries(rand_test_api_affine m pthread bscaler_api)
endif()

add_executable(rand_test_api_perspective
  ${CMAKE_CURRENT_SOURCE_DIR}/api_random/rand_test_perspective_api.cpp
  ${SRC_MDL}
  )
if (NOT MDL_ONLY)
  target_link_libraries(rand_test_api_perspective m pthread bscaler_api driver)
else()
  target_link_libraries(rand_test_api_perspective m pthread bscaler_api)
endif()

add_executable(rand_test_api
  ${CMAKE_CURRENT_SOURCE_DIR}/api_random/rand_resize_api.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/api_random/rand_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  )
if (NOT MDL_ONLY)
  target_link_libraries(rand_test_api m pthread bscaler_api driver)
else()
  target_link_libraries(rand_test_api m pthread bscaler_api)
endif()
