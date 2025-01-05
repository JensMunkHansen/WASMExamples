cmake_minimum_required(VERSION 3.12)
project(Assertions)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../CMake")
include(spsEmscriptenSettings)
include (CTest)
enable_testing()

sps_emscripten_module(
  TARGET_NAME example
  SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/AssertionTest.cxx
  ES6_MODULE OFF)

