cmake_minimum_required(VERSION 3.15)

set(TARGET_NAME ES6Test)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
include(EmscriptenSettings)

add_executable(${TARGET_NAME} ES6Test.cxx)

emscripten_settings(
  ES6_MODULE ON
  EXPORT_NAME "ES6Test"
  DEBUG "READABLE_JS"
  OPTIMIZATION "NONE"
  EMSCRIPTEN_LINK_OPTIONS emscripten_link_options
  EMSCRIPTEN_OPTIMIZATION_FLAGS emscripten_optimization_flags
  EMSCRIPTEN_DEBUG_INFO emscripten_debug_info)

message("Link options: ${emscripten_link_options}")
message("Optimization flags: ${emscripten_optimization_flags}")
message("Debug info: ${emscripten_debug_info}")
