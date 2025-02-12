cmake_minimum_required(VERSION 3.15)

project(CPPSideModule
  LANGUAGES C CXX)

include(CTest)
enable_testing()

set(TARGET_NAME CPPSideModuleTest)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../CMake)
include(spsEmscriptenDefaults)

if (NOT DEFINED WASMExamples_DEBUG)
  # We anticipate all options are set
  sps_set_emscripten_defaults(WASMExamples)
endif()
include(spsEmscriptenSettings)

# We get linker errors if this is C++

# WebAssembly SIDE_MODULEs cannot directly export C++ types to be used in another WASM module.
#
# This limitation stems from how WebAssembly is designed and how C++
# works at a low level. WebAssembly focuses on low-level constructs,
# and exporting complex C++ types directly (e.g., classes, structs, or
# templates) between modules would require sharing both type layouts
# and implementations, which WebAssembly does not natively support.

# Type library in ANSI-C
sps_emscripten_module(
  SIDE_MODULE
  VERBOSE
  ES6_MODULE OFF
  THREADING_ENABLED ON # Required for shared memory
  TARGET_NAME "TypeLibrary"
  EXPORTED_FUNCTIONS "CreateMyStruct;GetValue;SetValue"
  DEBUG ${WASMExamples_DEBUG}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  SOURCE_FILES "side.cxx")
sps_emscripten_module(
  MAIN_MODULE
  VERBOSE
  ES6_MODULE ON
  THREADING_ENABLED ON
  EMBIND ON
  EXPORT_NAME "loadCPPSideModuleTest"
  DEBUG ${WASMExamples_DEBUG}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  TARGET_NAME ${TARGET_NAME}
  SIDE_MODULES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/TypeLibrary.wasm"
  SOURCE_FILES "CPPSideModuleTest.cxx"
  JAVASCRIPT_FILES "run_CPPSideModuleTest.js")

add_test(NAME CPPSideModule_NoErrors
  COMMAND
  npx node run_CPPSideModuleTest.js
  WORKING_DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR})
