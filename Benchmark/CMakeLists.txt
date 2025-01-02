cmake_minimum_required(VERSION 3.14)

project(Benchmark
  LANGUAGES
  CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)

include(spsEmscriptenSettings)
include(spsHardware)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# Set specific compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -matomics -mbulk-memory")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -matomics -mbulk-memory")

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG v1.8.2
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    FIND_PACKAGE_ARGS 1.8.2)

FetchContent_MakeAvailable(benchmark)

find_package(Threads REQUIRED)

add_subdirectory(Pages)  
