# Reference the unmodified MIT checkout; shared by the exe and CPU link probe.
if(NOT TARGET librw)
  set(CSF_LIBRW_SOURCE "${CMAKE_SOURCE_DIR}/work/tools/librw" CACHE PATH "librw checkout")
  set(LIBRW_PLATFORM NULL CACHE STRING "CPU slice uses the no-device backend" FORCE)
  set(LIBRW_TOOLS OFF CACHE BOOL "" FORCE)
  set(LIBRW_INSTALL OFF CACHE BOOL "" FORCE)
  add_subdirectory("${CSF_LIBRW_SOURCE}" "${CMAKE_BINARY_DIR}/librw" EXCLUDE_FROM_ALL)
  # Upstream Xbox unsigned {-1} tables: disable narrowing without a source patch.
  target_compile_options(librw PRIVATE -Wno-c++11-narrowing)
  # Upstream Clang adds Unix libm even for clang-cl targeting Windows.
  if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    get_target_property(_rwlibs librw LINK_LIBRARIES)
    list(REMOVE_ITEM _rwlibs m)
    set_property(TARGET librw PROPERTY LINK_LIBRARIES "${_rwlibs}")
    get_target_property(_rwinterface librw INTERFACE_LINK_LIBRARIES)
    list(REMOVE_ITEM _rwinterface "$<LINK_ONLY:m>")
    set_property(TARGET librw PROPERTY INTERFACE_LINK_LIBRARIES "${_rwinterface}")
  endif()
endif()
