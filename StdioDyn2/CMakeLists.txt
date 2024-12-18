cmake_minimum_required(VERSION 3.22...3.28)

project(Stdio)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)
include(spsEmscriptenSettings)

if (EMSCRIPTEN)
  sps_emscripten_module(
    ES6_MODULE OFF
    VERBOSE
    #FILE_SYSTEM ON
    ASYNCIFY OFF
    EXIT_RUNTIME OFF
    EXPORTED_FUNCTIONS malloc;free
    TARGET_NAME example
    SOURCE_FILES example.cpp
    JAVASCRIPT_FILES "stdio.js"
    OPTIMIZATION NONE
    DEBUG DEBUG_NATIVE
  )
endif()
