#if (${BUILD_ENV} STREQUAL "CSE")
#  include_directories(
#    ${CMAKE_CURRENT_SOURCE_DIR}/driver/cse
# 	)
#  aux_source_directory(${PROJECT_SOURCE_DIR}/driver/cse SRC_DRIVER)
#  add_library(driver STATIC ${SRC_DRIVER} ${SRC_HW_API})	
#elseif (${BUILD_ENV} STREQUAL "EYER")
#  include_directories(
#    ${CMAKE_CURRENT_SOURCE_DIR}/driver/eyer
# 	)
#  aux_source_directory(${PROJECT_SOURCE_DIR}/driver/eyer SRC_EYER)
#  add_library(driver STATIC ${SRC_EYER} ${SRC_HW_API})	
#  set_target_properties(driver PROPERTIES OUTPUT_NAME "driver" COMPILE_FLAGS "-m32 -lm -O4 -Wall -g -ffast-math -DHAVE_MALLOC_H -DARCH_X86 -DSYS_LINUX -s -fomit-frame-pointer -s -lpthread")
#endif()


if (${BUILD_ENV} STREQUAL "EYER")
  include_directories(
	${PROJECT_SOURCE_DIR}/include/platform/eyer
	)
  aux_source_directory(${PROJECT_SOURCE_DIR}/source/platform/eyer SRC_EYER)
  add_library(driver STATIC ${SRC_EYER} ${SRC_HW_API})	
  set_target_properties(driver PROPERTIES OUTPUT_NAME "driver" COMPILE_FLAGS "-m32 -lm -O4 -Wall -g -ffast-math -DHAVE_MALLOC_H -DARCH_X86 -DSYS_LINUX -s -fomit-frame-pointer -s -lpthread")
endif()

