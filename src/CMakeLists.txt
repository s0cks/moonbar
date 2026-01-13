list(APPEND BAR_SOURCES
  version.c
  state.c
  state_lua.c
  state_debug.c
  state_gtk.c)

add_library(${PROJECT_NAME}-core SHARED ${BAR_SOURCES})
target_include_directories(${PROJECT_NAME}-core
  PUBLIC ${BAR_INCLUDE_DIR}
         ${LuaJIT_INCLUDE_DIRS}
  PRIVATE
         ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}-core
  PUBLIC PkgConfig::GTK 
         PkgConfig::GTK_LAYER_SHELL
         PkgConfig::LuaJIT)
add_library(${PROJECT_NAME}::core ALIAS ${PROJECT_NAME}-core)
