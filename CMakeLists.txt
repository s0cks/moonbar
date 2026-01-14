cmake_minimum_required(VERSION 3.29)
project(moonbar
  VERSION 0.0.1
  HOMEPAGE_URL https://github.com/s0cks/moonbar
  LANGUAGES C)

set(MBAR_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
if(CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
  add_compile_definitions(MBAR_DEBUG)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk4>=4.14)
pkg_check_modules(GTK_LAYER_SHELL REQUIRED IMPORTED_TARGET gtk4-layer-shell-0>=1.3.0)
pkg_check_modules(LuaJIT REQUIRED IMPORTED_TARGET luajit)

message(STATUS "luajit include dirs: ${LuaJIT_INCLUDE_DIRS}")

add_subdirectory(src)

# option(BUILD_BINDINGS "Build lua binding libs." ON)
# if(BUILD_BINDINGS)
#   add_subdirectory(bindings/)
# endif()

option(ENABLE_TESTS "Enable tests." OFF)
if(ENABLE_TESTS)
  add_subdirectory(tests)
endif()

option(BUILD_EXECUTABLE "Build the executable." ON)
if(BUILD_EXECUTABLE)
  add_executable(${PROJECT_NAME}
    main.c)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
      ${PROJECT_NAME}::core)
endif()
