cmake_minimum_required(VERSION 3.15)

project(VTKABITest
  LANGUAGES C CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Ensures no compiler-specific extensions like GNU++17 are used

include(CTest)
enable_testing()

find_package(VTK
  COMPONENTS
  vtkCommonCore)

set(TARGET_NAME VTKABITest)

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

set(SIDE_TARGET_NAME SideModule)
sps_emscripten_module(
  SIDE_MODULE
  VERBOSE
  ES6_MODULE OFF
  THREADING_ENABLED ON # Required for shared memory
  TARGET_NAME ${SIDE_TARGET_NAME}
  EXPORTED_FUNCTIONS "Process;ArrayCreate;ArrayDelete;ArrayReferenceCounterGet;ArrayReferenceCounterSet;ArrayDataPointerGet;ArrayDataPointerSet;ArrayShallowCopy;ArrayNumberOfTuplesGet;ArrayNumberOfTuplesSet;ArrayNumberOfComponentsGet;ArrayNumberOfComponentsSet;ArrayResize;ArrayElementTypeGet;ArrayUpdateCallbackSet;ArrayElementSizeGet"
  #EXPORTED_FUNCTIONS "Process"
  DEBUG ${WASMExamples_DEBUG}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION}
#  SOURCE_FILES "vector_abi.cxx;side_abi.cxx")
  SOURCE_FILES "array_abi.cxx;side_array_abi.cxx")
message("CMAKE_CFG_INTDIR: ${CMAKE_CFG_INTDIR}")

sps_emscripten_module(
  MAIN_MODULE
  VERBOSE
  ES6_MODULE ON
  THREADING_ENABLED ON
  EMBIND ON
  EXPORT_NAME "loadSideModuleTest"
  DEBUG ${WASMExamples_DEBUG}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  LIBRARIES ${VTK_LIBRARIES}
  TARGET_NAME ${TARGET_NAME}
  SIDE_MODULES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/SideModule.wasm"
#  SOURCE_FILES "host.cxx;vector_abi.cxx"
  SOURCE_FILES "host_array.cxx"#;array_abi.cxx"
  JAVASCRIPT_FILES "run_SideModuleTest.js")

add_test(NAME ES6_SideModule_NoErrors
  COMMAND
  npx node run_SideModuleTest.js
  WORKING_DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR})
add_dependencies(${TARGET_NAME} ${SIDE_TARGET_NAME})
