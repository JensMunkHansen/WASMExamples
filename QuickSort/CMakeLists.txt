cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)

# Project name
project(QuickSort
  LANGUAGES C CXX)

include(spsEmscriptenDefaults)

if (NOT DEFINED WASMExamples_DEBUG)
  # We anticipate all options are set
  sps_set_emscripten_defaults(WASMExamples)
endif()

include(spsEmscriptenSettings)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(ENABLE_EXCEPTIONS OFF)

set(TARGET_NAME quicksort)
if (EMSCRIPTEN)
  sps_emscripten_module(
    ES6_MODULE OFF
    SOURCE_FILES QuickSort.cxx
    TARGET_NAME ${TARGET_NAME}
    THREADING_ENABLED ON
    DEBUG ${WASMExamples_DEBUG}
    COMPILE_OPTIMIZATION ${WASMExamples_COMPILE_OPTIMIZATION}
    OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  )
endif()

# Enable testing
include(CTest)

