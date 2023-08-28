set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

#find package openmp for mutli-progress
if (${BUILD_ENV} STREQUAL "X86")
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
  ${PROJECT_SOURCE_DIR}/include/platform
  ${PROJECT_SOURCE_DIR}/include/3rd_party
  ${PROJECT_SOURCE_DIR}/include/api
  ${PROJECT_SOURCE_DIR}/include/api/hal
  ${PROJECT_SOURCE_DIR}/include/model
  ${PROJECT_SOURCE_DIR}/include/model/golden
  ${PROJECT_SOURCE_DIR}/include/wrap #delete me
  ${PROJECT_SOURCE_DIR}/tools/random_api
)

if (${BUILD_ENV} STREQUAL "CSE")
  include_directories(
    ${PROJECT_SOURCE_DIR}/driver/cse
  )
elseif (${BUILD_ENV} STREQUAL "EYER")
  include_directories(
    ${PROJECT_SOURCE_DIR}/include/platform/eyer
	)
elseif (${BUILD_ENV} STREQUAL "X86")
  add_subdirectory(model)
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/source/model/golden SRC_MDL)

if (NOT MDL_ONLY)
  add_subdirectory(api)
  #add_subdirectory(vector)
  add_subdirectory(basic)
endif()

add_subdirectory(performance)
