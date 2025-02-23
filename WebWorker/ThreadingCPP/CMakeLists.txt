cmake_minimum_required(VERSION 3.22...3.28)

project(ThreadingCPP)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../CMake)

include(spsEmscriptenDefaults)

if (NOT DEFINED WASMExamples_DEBUGINFO)
  # We anticipate all options are set
  sps_set_emscripten_defaults(WASMExamples)
endif()
include(spsEmscriptenSettings)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

sps_emscripten_module(
  ES6_MODULE ON
  EMBIND ON
  THREADING_ENABLED ON
  JAVASCRIPT_FILES index.html main.mjs worker.mjs
  TARGET_NAME ThreadingCPP
  SOURCE_FILES library.cpp
  DEBUG ${WASMExamples_DEBUGINFO}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION}
)
