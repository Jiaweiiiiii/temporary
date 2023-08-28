set(OPT_LEVEL "-O2")
#set(CSE_CFLAGS "-DUSE_SIM")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPT_LEVEL} ${CSE_CFLAGS} -EL -mips32r2 -mfp64 -mabs=2008 -mnan=2008 -fPIC -ffunction-sections -fdata-sections")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRCS)
include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

add_library(nna_static STATIC ${SRCS})
add_library(nna SHARED ${SRCS})

set_target_properties(nna_static PROPERTIES OUTPUT_NAME "nna")
set_target_properties(nna PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(nna PROPERTIES VERSION ${MAJOR}.${MINOR}.${PATCH})

install(TARGETS nna DESTINATION ${CMAKE_INSTALL_PREFIX}/nna/lib)
install(TARGETS nna_static DESTINATION ${CMAKE_INSTALL_PREFIX}/nna/lib)
set(INSTALL_HEAD_FILES
  ${PROJECT_SOURCE_DIR}/include/aie_bscaler.h
  ${PROJECT_SOURCE_DIR}/include/aie_mmap.h
  ${PROJECT_SOURCE_DIR}/include/aie_nndma.h
  ${PROJECT_SOURCE_DIR}/include/dma_func.h
  ${PROJECT_SOURCE_DIR}/include/ddr_mem.h
  ${PROJECT_SOURCE_DIR}/include/ivspmon.h
  ${PROJECT_SOURCE_DIR}/include/libxio.h
  ${PROJECT_SOURCE_DIR}/include/macro_def.h
  ${PROJECT_SOURCE_DIR}/include/mxu3.h
  ${PROJECT_SOURCE_DIR}/include/nna.h
  ${PROJECT_SOURCE_DIR}/include/nna_app.h
  ${PROJECT_SOURCE_DIR}/include/nna_dma_driver_v4.h
  ${PROJECT_SOURCE_DIR}/include/nna_regs.h
  ${PROJECT_SOURCE_DIR}/include/nna_dma_memory_v1.h
  ${PROJECT_SOURCE_DIR}/include/nna_reg_set_v.h
  ${PROJECT_SOURCE_DIR}/include/nna_reg_set_c.h
  ${PROJECT_SOURCE_DIR}/include/oram_mem.h
  ${PROJECT_SOURCE_DIR}/include/soc_nna.h
  )
install(FILES ${INSTALL_HEAD_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/nna/include)
