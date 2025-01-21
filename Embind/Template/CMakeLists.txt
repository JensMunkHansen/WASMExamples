cmake_minimum_required(VERSION 3.15)
project(Template
LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../CMake)

include(CTest)
enable_testing()

find_package(VTK 
  COMPONENTS
  CommonCore
)

set(TARGET_TEST_NAME main)

include(spsEmscriptenDefaults)
include(spsAsan)
if (NOT DEFINED WASMExamples_DEBUG)
  # We anticipate all options are set
  sps_set_emscripten_defaults(WASMExamples)
endif()
include(spsEmscriptenSettings)

sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES main.cpp
  JAVASCRIPT_FILES run.js
  EMBIND ON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/template.html ${CMAKE_CURRENT_BINARY_DIR}/index.html @ONLY)

add_test(NAME TemplatedFunction_Call_NoExceptions
  COMMAND
    npm run test
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR})
      
