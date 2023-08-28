### example ####
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

include_directories(
  ${PROJECT_SOURCE_DIR}/include/api
  ${PROJECT_SOURCE_DIR}/include/api/hal
  ${PROJECT_SOURCE_DIR}/include/model
  ${PROJECT_SOURCE_DIR}/include/model/golden
  ${PROJECT_SOURCE_DIR}/include/3rd_party
)

add_executable(gen_c_vector
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/gen_c_vector.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/dump_c_vector.c
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/random_vector.c
)
target_link_libraries(gen_c_vector bscaler_api bscaler_mdl)

add_executable(gen_c_chain_vector
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/gen_c_chain_vector.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/dump_c_vector.c
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/random_vector.c
)
target_link_libraries(gen_c_chain_vector bscaler_api bscaler_mdl)

add_executable(gen_t_vector
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/gen_t_vector.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/dump_t_vector.c
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/random_vector.c
  )
target_link_libraries(gen_t_vector bscaler_api bscaler_mdl m)

add_executable(gen_t_chain_vector
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/gen_t_chain_vector.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/dump_t_vector.c
  ${CMAKE_CURRENT_SOURCE_DIR}/random_vector/random_vector.c
)
target_link_libraries(gen_t_chain_vector bscaler_api bscaler_mdl)


add_executable(gen_random_bin
  ${CMAKE_CURRENT_SOURCE_DIR}/random_api/gen_random_bin.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/random_api/random_api.cpp
  ${PROJECT_SOURCE_DIR}/source/3rd_party/Matrix.cpp
  )
target_link_libraries(gen_random_bin bscaler_api bscaler_mdl)
