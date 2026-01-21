cmake_minimum_required(VERSION 3.29)
project(moonbar
  VERSION 0.0.1
  HOMEPAGE_URL https://github.com/s0cks/moonbar
  LANGUAGES C)

set(MOONBAR_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/moonbar/)
if(CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
  add_compile_definitions(MBAR_DEBUG)
  add_compile_definitions(_NDEBUG)
endif()

set(MOONBAR_IWYU_MAPPINGS "${CMAKE_SOURCE_DIR}/iwyu.imp")
include(IWYU)

find_package(libuv CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk4>=4.14)
pkg_check_modules(GTK_LAYER_SHELL REQUIRED IMPORTED_TARGET gtk4-layer-shell-0>=1.3.0)
pkg_check_modules(LuaJIT REQUIRED IMPORTED_TARGET luajit)

add_library(${PROJECT_NAME}-api INTERFACE)
target_include_directories(${PROJECT_NAME}-api
  INTERFACE ${MOONBAR_INCLUDE_DIR}
            ${LuaJIT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}-api
  INTERFACE PkgConfig::GTK 
            PkgConfig::GTK_LAYER_SHELL
            PkgConfig::LuaJIT
            $<IF:$<TARGET_EXISTS:libuv::uv_a>,libuv::uv_a,libuv::uv>)
add_library(${PROJECT_NAME}::api ALIAS ${PROJECT_NAME}-api)

add_subdirectory(lib/)
add_subdirectory(src/)

option(ENABLE_TESTS "Enable tests." OFF)
if(ENABLE_TESTS)
  add_subdirectory(tests)
endif()

option(BUILD_MODULES "Build the modules." ON)
if(BUILD_MODULES)
  add_subdirectory(modules/)
endif()

option(BUILD_EXECUTABLE "Build the executable." ON)
if(BUILD_EXECUTABLE)
  add_executable(${PROJECT_NAME}
    main.c)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
      ${PROJECT_NAME}::core)
  target_enable_iwyu(${PROJECT_NAME} ${MOONBAR_IWYU_MAPPINGS})
endif()
