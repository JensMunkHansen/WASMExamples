cmake_minimum_required(VERSION 3.15)

project(ES6Test
  LANGUAGES C CXX)

set(TARGET_NAME ES6Test)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)
include(EmscriptenSettings)

emscripten_module(
  ES6_MODULE ON
  EMBIND ON
  EXPORT_NAME "loadES6Test"
  DEBUG "READABLE_JS"
  OPTIMIZATION "NONE"
  TARGET_NAME ${TARGET_NAME}
  SOURCE_FILES "ES6Test.cxx"
  JAVASCRIPT_FILES "run_ES6Test.js")

