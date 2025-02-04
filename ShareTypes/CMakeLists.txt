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
  JAVASCRIPT_FILES runc.js
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadFirstModule
  EXPORTED_FUNCTIONS CreateStruct;DestroyStruct;Produce;Consume;ConsumeTest
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})

set(TARGET_TEST_NAME Second)

sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx Second.cxx
  JAVASCRIPT_FILES runc.js
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadSecondModule
  EXPORTED_FUNCTIONS CreateStruct;DestroyStruct;Produce;Consume;ConsumeTest
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
  COMPILE_DEFINITIONS CPPTYPES
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadFirstCPPModule
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})

set(TARGET_TEST_NAME SecondCPP)

sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx Second.cxx
  JAVASCRIPT_FILES runcpp.js
  EMBIND ON
  COMPILE_DEFINITIONS CPPTYPES
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadSecondCPPModule
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})

set(TARGET_TEST_NAME FirstSharedCPP)

# Two modules with C++ interfaces (shared memory)
sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx First.cxx
  JAVASCRIPT_FILES runshared.js
  THREADING_ENABLED ON
  EMBIND ON
  COMPILE_DEFINITIONS CPPTYPES
  EXPORT_NAME loadFirstCPPModule
  DEBUG ${WASMExamples_DEBUG}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})
# Specify linker file (not honored in WASM yet)
#target_link_options(${TARGET_TEST_NAME} PRIVATE "--ldflags=${CMAKE_CURRENT_SOURCE_DIR}/First.ld")

set(TARGET_TEST_NAME SecondSharedCPP)

sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx Second.cxx
  JAVASCRIPT_FILES runshared.js
  THREADING_ENABLED ON
  EMBIND ON
  COMPILE_DEFINITIONS CPPTYPES
  EXPORT_NAME loadSecondCPPModule
  DEBUG ${WASMExamples_DEBUG}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})
# Specify linker file (not honored in WASM yet)
#target_link_options(${TARGET_TEST_NAME} PRIVATE "--ldflags=${CMAKE_CURRENT_SOURCE_DIR}/Second.ld")


set(TARGET_TEST_NAME FirstShared)
  
# Two modules with C++ interfaces
sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx First.cxx
  JAVASCRIPT_FILES runshared.js
  EMBIND ON
  COMPILE_DEFINITIONS CPPTYPES
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadFirstSharedModule
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})

set(TARGET_TEST_NAME SecondShared)

sps_emscripten_module(
  TARGET_NAME ${TARGET_TEST_NAME}
  ES6_MODULE ON
  SOURCE_FILES Types.cxx First.cxx
  JAVASCRIPT_FILES runshared.js
  EMBIND ON
  COMPILE_DEFINITIONS CPPTYPES
  DEBUG ${WASMExamples_DEBUG}
  EXPORT_NAME loadSecondSharedModule
  OPTIMIZATION ${WASMExamples_OPTIMIZATION})




