cmake_minimum_required(VERSION 3.15)

project(ShareTypes
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)

include(CTest)
enable_testing()

set(TARGET_TEST_NAME First)

include(spsEmscriptenDefaults)
include(spsAsan)
include(spsUBSan)

if (NOT DEFINED WASMExamples_DEBUG)
  # We anticipate all options are set
  sps_set_emscripten_defaults(WASMExamples)
endif()
include(spsEmscriptenSettings)

# Two modules with pure C interfaces
sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx First.cxx
  JAVASCRIPT_FILES run.js
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadFirstModule
  EXPORTED_FUNCTIONS CreateStruct;Produce;Consume;ConsumeTest
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})

set(TARGET_TEST_NAME Second)

sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx First.cxx
  JAVASCRIPT_FILES run.js
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadSecondModule
  EXPORTED_FUNCTIONS CreateStruct;Produce;Consume;ConsumeTest
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})

add_test(NAME SharingCTypesDataSharingFails
  COMMAND
    ${CMAKE_COMMAND} -E env
    "CMAKE_CONFIG_TYPE=$<CONFIG>"  
    npm run test
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

set(TARGET_TEST_NAME FirstCPP)
  
# Two modules with C++ interfaces
sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx First.cxx
  JAVASCRIPT_FILES runcpp.js
  EMBIND ON
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadFirstCPPModule
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})

set(TARGET_TEST_NAME SecondCPP)

sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx First.cxx
  JAVASCRIPT_FILES runcpp.js
  EMBIND ON
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadSecondCPPModule
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})


