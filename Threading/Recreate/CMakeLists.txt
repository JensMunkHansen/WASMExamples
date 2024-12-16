cmake_minimum_required(VERSION 3.15)

project(MultipleThreads)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../CMake)
include(spsEmscriptenSettings)

sps_emscripten_module(
  ES6_MODULE             OFF
  VERBOSE
  THREADING_ENABLED      ON
  THREAD_POOL_SIZE       4
  TARGET_NAME            MultipleThreads
  SOURCE_FILES           "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
  OPTIMIZATION           NONE
  DEBUG                  READABLE_JS)
