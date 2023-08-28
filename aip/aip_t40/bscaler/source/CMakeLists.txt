add_subdirectory(api)

if (${BUILD_ENV} STREQUAL "EYER")
  add_subdirectory(platform/eyer)
endif()

add_subdirectory(model)
