#add_executable(t_bscaler_c
#  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_c.c
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  )
#target_link_libraries(t_bscaler_c m pthread bscaler_api driver)
# 
##add_executable(t_bscaler_chain_c
##  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_chain_c.c
##  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
##  )
##target_link_libraries(t_bscaler_chain_c m pthread bscaler_api driver)
 
add_executable(t_bscaler_t
  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_t.c
  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
  )
target_link_libraries(t_bscaler_t m pthread bscaler_api driver)
 
##add_executable(t_bscaler_chain_t
##  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_chain_t.c
##  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
##  )
##target_link_libraries(t_bscaler_chain_t m pthread bscaler_api driver)
# 
#add_executable(t_bscaler_c_t
#  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_c_t.c
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  )
#target_link_libraries(t_bscaler_c_t m pthread bscaler_api driver)
# 
##add_executable(t_bscaler_chain_c_t
##  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_chain_c_t.c
##  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
##  )
##target_link_libraries(t_bscaler_chain_c_t m pthread bscaler_api driver)
# 
##add_executable(t_lzma
##  ${CMAKE_CURRENT_SOURCE_DIR}/t_lzma.c
##  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
##  )
##target_link_libraries(t_lzma m pthread bscaler_api driver)
# 
#add_executable(t_bscaler_reg_rw
#  ${CMAKE_CURRENT_SOURCE_DIR}/t_bscaler_reg_rw.c
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  )
#target_link_libraries(t_bscaler_reg_rw m pthread bscaler_api driver)



########################################################################
## eyer case
########################################################################

#add_executable(t_bscaler_c_eyer
#  ${CMAKE_CURRENT_SOURCE_DIR}/eyer/t_bscaler_c.c
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  )
#target_link_libraries(t_bscaler_c_eyer m pthread bscaler_api driver)

#add_executable(t_bscaler_chain_c_eyer
#  ${CMAKE_CURRENT_SOURCE_DIR}/eyer/t_bscaler_chain_c.c
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  )
#target_link_libraries(t_bscaler_chain_c_eyer m pthread bscaler_api driver)
 
#add_executable(t_bscaler_t_eyer
#  ${CMAKE_CURRENT_SOURCE_DIR}/eyer/t_bscaler_t.c
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  )
#target_link_libraries(t_bscaler_t_eyer m pthread bscaler_api driver)
 
#add_executable(t_bscaler_chain_t_eyer
#  ${CMAKE_CURRENT_SOURCE_DIR}/eyer/t_bscaler_chain_t.c
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  )
#target_link_libraries(t_bscaler_chain_t_eyer m pthread bscaler_api driver)
 
#add_executable(t_bscaler_c_t_eyer
#  ${CMAKE_CURRENT_SOURCE_DIR}/eyer/t_bscaler_c_t.c
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  )
#target_link_libraries(t_bscaler_c_t_eyer m pthread bscaler_api driver)
 
#add_executable(t_bscaler_chain_c_t_eyer
#  ${CMAKE_CURRENT_SOURCE_DIR}/eyer/t_bscaler_chain_c_t.c
#  ${PROJECT_SOURCE_DIR}/source/platform/platform.c
#  )
#target_link_libraries(t_bscaler_chain_c_t_eyer m pthread bscaler_api driver)
