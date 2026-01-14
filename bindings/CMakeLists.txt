macro(add_binding name sources)
  set(bindings_name "${PROJECT_NAME}-${name}-bindings")
  add_library(${bindings_name} SHARED ${sources})
  set_target_properties(${bindings_name}
    PROPERTIES
      OUTPUT_NAME "${name}_bindings"
      PREFIX "")
  target_include_directories(${bindings_name}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${bindings_name}
    PUBLIC ${PROJECT_NAME}::core)
  add_library(${PROJECT_NAME}::${name}-bindings
    ALIAS ${bindings_name})
endmacro()

add_subdirectory(bar/)
add_subdirectory(button/)
add_subdirectory(label/)
