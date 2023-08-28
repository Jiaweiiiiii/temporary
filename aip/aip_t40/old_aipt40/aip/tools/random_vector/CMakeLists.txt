### example ####
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

include_directories(
  ${PROJECT_SOURCE_DIR}/include/api/hal
  ${PROJECT_SOURCE_DIR}/include/3rd_party
  ${PROJECT_SOURCE_DIR}
)

add_executable(gen_c_vector
  ${CMAKE_CURRENT_SOURCE_DIR}/gen_c_vector.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/dump_c_vector.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bscaler_random.c
)
target_link_libraries(gen_c_vector m pthread bscaler_api)

add_executable(gen_c_chain_vector
  ${CMAKE_CURRENT_SOURCE_DIR}/gen_c_chain_vector.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/dump_c_vector.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bscaler_random.c
)
target_link_libraries(gen_c_chain_vector m pthread bscaler_api)

add_executable(gen_t_vector
  ${CMAKE_CURRENT_SOURCE_DIR}/gen_t_vector.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/dump_t_vector.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bscaler_random.c
)
target_link_libraries(gen_t_vector m pthread bscaler_api)

add_executable(gen_t_chain_vector
  ${CMAKE_CURRENT_SOURCE_DIR}/gen_t_chain_vector.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/dump_t_vector.c
  ${CMAKE_CURRENT_SOURCE_DIR}/bscaler_random.c
)
target_link_libraries(gen_t_chain_vector m pthread bscaler_api)
