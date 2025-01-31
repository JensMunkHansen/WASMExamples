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
if (NOT VTK_FOUND)
  message("VTK not found skipping")
  return()
endif()

set(TARGET_TEST_NAME main)

include(spsEmscriptenDefaults)
include(spsAsan)
include(spsUBSan)

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
  DEBUG ${WASMExamples_DEBUG}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  EMBIND ON)

target_compile_definitions(${TARGET_TEST_NAME} PRIVATE
    $<$<CONFIG:Asan>:__SANITIZE_ADDRESS__=1>
)
target_compile_definitions(${TARGET_TEST_NAME} PRIVATE
    $<$<CONFIG:UBsan>:__SANITIZE_UNDEFINED__=1>
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/template.html ${CMAKE_CURRENT_BINARY_DIR}/index.html @ONLY)

add_test(NAME TemplatedFunction_Call_NoExceptions
  COMMAND
    ${CMAKE_COMMAND} -E env
    "CMAKE_CONFIG_TYPE=$<CONFIG>"  
    npm run test
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
      
