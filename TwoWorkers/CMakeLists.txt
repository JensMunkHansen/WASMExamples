cmake_minimum_required(VERSION 3.22...3.28)

project(TwoWorkers)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)
include(EmscriptenSettings)

option(64_BIT OFF)
option(THREADING_ENABLED ON)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (EMSCRIPTEN)
  sps_emscripten_module(
    64_BIT ${64_BIT}
    ES6_MODULE ON
    VERBOSE
    THREADING_ENABLED ${THREADING_ENABLED}
    THREAD_POOL_SIZE 4
    TARGET_NAME example
    EXPORT_NAME "CustomModuleName"
    EXPORTED_FUNCTIONS "add"
    SOURCE_FILES main.cxx
    JAVASCRIPT_FILES "main.js;worker.js"
    OPTIMIZATION NONE
    LIBRARIES ${LIBRARY_NAME}
    DEBUG PROFILE
  )
endif()
