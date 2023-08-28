add_executable(read_all_reg
  ${CMAKE_CURRENT_SOURCE_DIR}/read_all_reg.c
  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
  )
target_link_libraries(read_all_reg bscaler_api ${NNA_DRIVER} m pthread)

add_executable(bst_reset
  ${CMAKE_CURRENT_SOURCE_DIR}/bst_reset.c
  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
  )
target_link_libraries(bst_reset bscaler_api ${NNA_DRIVER} m pthread)

add_executable(bsc_reset
  ${CMAKE_CURRENT_SOURCE_DIR}/bsc_reset.c
  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
  )
target_link_libraries(bsc_reset bscaler_api ${NNA_DRIVER} m pthread)
