cmake_minimum_required(VERSION 3.15)

project(ShareVTKTypes
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)

include(CTest)
enable_testing()


include(spsEmscriptenDefaults)
include(spsAsan)
include(spsUBSan)

if (NOT DEFINED WASMExamples_DEBUGINFO)
  # We anticipate all options are set
  sps_set_emscripten_defaults(WASMExamples)
endif()
include(spsEmscriptenSettings)

set(TARGET_TEST_NAME First)

# Two modules with C++ interfaces
sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Factory.cxx First.cxx
  JAVASCRIPT_FILES runc.js
  EXPORT_NAME loadFirstModule
  DEBUG ${WASMExamples_DEBUGINFO}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})

set(TARGET_TEST_NAME Second)

sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Factory.cxx Second.cxx
  JAVASCRIPT_FILES runc.js
  EXPORT_NAME loadSecondModule
  EXPORTED_FUNCTIONS CreateStruct;DestroyStruct;Produce;Consume;ConsumeTest
  DEBUG ${WASMExamples_DEBUGINFO}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})

add_test(NAME SharingVTKTypes
  COMMAND
    ${CMAKE_COMMAND} -E env
    "CMAKE_CONFIG_TYPE=$<CONFIG>"  
    npm run test
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

