#TODO: Update
cmake_minimum_required(VERSION 3.22...3.28)

project(Catch2Test)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)
include(EmscriptenSettings)

option(64_BIT OFF)
option(THREADING_ENABLED OFF)
set(_THREADING_ENABLED 0)
if (THREADING_ENABLED)
  set(_THREADING_ENABLED 1)
endif()

set(TARGET_NAME main)
set(LIBRARY_NAME library)
set(LIBRARY_PROXY_NAME wasmlibrary)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (BUILD_TESTING)
  find_package(Catch2 3.5.2 EXACT REQUIRED)
  include(CTest)
  include(Catch)
  enable_testing()
endif()

include(GenerateExportHeader)

# Configuration file for enabling/disabling threading
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

add_library(${LIBRARY_NAME} STATIC "")

generate_export_header(${LIBRARY_NAME})

target_sources(${LIBRARY_NAME}
  PUBLIC
    FILE_SET public_headers
    TYPE HEADERS
    FILES
    library.h
  PRIVATE
    library.cxx
)

target_include_directories(${LIBRARY_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

# 64-bit
if (64_BIT) 
  target_compile_options(${LIBRARY_NAME} PRIVATE "-target=wasm64")
endif()

if (THREADING_ENABLED)
  target_link_libraries(${LIBRARY_NAME} PRIVATE Threads::Threads)

  target_compile_options(${LIBRARY_NAME} PRIVATE 
    -matomics 
    -mbulk-memory)  
endif()



if (EMSCRIPTEN)
  sps_emscripten_module(
    64_BIT ${64_BIT}
    ES6_MODULE ON
    VERBOSE
    THREADING_ENABLED ${THREADING_ENABLED}
    THREAD_POOL_SIZE 4
    TARGET_NAME ${LIBRARY_PROXY_NAME}
    EXPORTED_FUNCTIONS "DoWork"
    SOURCE_FILES dummy.cxx
    JAVASCRIPT_FILES "index.html;run_Catch2Test.js"
    EXPORT_NAME "loadCatch2TestModule"
    OPTIMIZATION NONE
    LIBRARIES ${LIBRARY_NAME}
    DEBUG PROFILE
  )
  target_compile_options(${LIBRARY_PROXY_NAME} PRIVATE 
    -matomics 
    -mbulk-memory)  
else()
  add_executable(${TARGET_NAME} main.cxx)
  target_link_libraries(${TARGET_NAME} PRIVATE ${LIBRARY_NAME}) 
endif()

if (BUILD_TESTING)
  add_test(NAME Catch2Test_ThreadedExecution_NoErrors
    COMMAND
    npx node run_Catch2Test.js
    WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR})
endif()

if (BUILD_TESTING)
  if (EMSCRIPTEN)
    set(LIBRARY_LIST ${LIBRARY_NAME} Catch2::Catch2WithMain)
    sps_emscripten_module(
      VERBOSE
      64_BIT ${64_BIT}
      TARGET_NAME sampleTest
      SOURCE_FILES test.cxx
      LIBRARIES ${LIBRARY_NAME} Catch2::Catch2WithMain
      ES6_MODULE OFF
      THREADING_ENABLED ${THREADING_ENABLED})
  else()
    add_executable(sampleTest test.cxx)
    target_link_libraries(sampleTest PRIVATE library)
    target_link_libraries(sampleTest PRIVATE Catch2::Catch2WithMain)
  endif()
  catch_discover_tests(sampleTest)
endif()
# npx node sampleTest.cjs

