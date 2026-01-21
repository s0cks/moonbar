set(LIB_NAME bluetooth)
file(GLOB LIB_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(${PROJECT_NAME}-${LIB_NAME} SHARED
  ${LIB_SOURCES})
set_target_properties(${PROJECT_NAME}-${LIB_NAME}
  PROPERTIES
    OUTPUT_NAME ${LIB_NAME}
    PREFIX "")
target_include_directories(${PROJECT_NAME}-${LIB_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}-${LIB_NAME}
  PUBLIC ${PROJECT_NAME}::api)
