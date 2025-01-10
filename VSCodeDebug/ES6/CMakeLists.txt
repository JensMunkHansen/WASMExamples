cmake_minimum_required(VERSION 3.22...3.28)

project(ES6)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../CMake)
include(spsEmscriptenSettings)

sps_set_emscripten_defaults(${PROJECT_NAME})

set(TARGET_NAME ES6)
# C/C++ callback
sps_emscripten_module(
  ES6_MODULE ON
  VERBOSE
  THREADING_ENABLED OFF
  TARGET_NAME ${TARGET_NAME}
  JAVASCRIPT_FILES run_ES6.js index.html favicon.ico
  SOURCE_FILES Simple.cxx
  EXPORT_NAME loadES6Module
  EXPORTED_FUNCTIONS HelloWorld
  OPTIMIZATION NONE
  DEBUG ${${PROJECT_NAME}_DEBUG}
)

# Add a post-build custom command (symbols have root in this folder!!)
add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Simple.cxx"
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/"       # Destination directory
    COMMENT "Copying Simple.cxx to output directory"
)
