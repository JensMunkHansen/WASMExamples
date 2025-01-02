cmake_minimum_required(VERSION 3.14)

project(Pages LANGUAGES CXX)

sps_get_processor_count(MAX_CONCURRENCY)

set(TARGET_NAME pages)
if (EMSCRIPTEN)
  sps_emscripten_module(
    TARGET_NAME ${TARGET_NAME}
    SOURCE_FILES benchmark.cpp
    THREAD_POOL_SIZE ${MAX_CONCURRENCY}
    ES6_MODULE OFF
    EXIT_RUNTIME ON
    OPTIMIZATION BEST
    LIBRARIES benchmark::benchmark;Threads::Threads
    FILE_SYSTEM ON
    PRE_JS ${CMAKE_CURRENT_SOURCE_DIR}/pre.js
    THREADING_ENABLED ON)
endif()



