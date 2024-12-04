cmake_minimum_required(VERSION 3.15)

project(ES6Test
  LANGUAGES C CXX)

set(TARGET_NAME ES6Test)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)
include(EmscriptenSettings)

# We get linker errors if this is C++

# WebAssembly SIDE_MODULEs cannot directly export C++ types to be used in another WASM module.
#
# This limitation stems from how WebAssembly is designed and how C++
# works at a low level. WebAssembly focuses on low-level constructs,
# and exporting complex C++ types directly (e.g., classes, structs, or
# templates) between modules would require sharing both type layouts
# and implementations, which WebAssembly does not natively support.

# Type library in ANSI-C
emscripten_module(
  SIDE_MODULE
  ES6_MODULE OFF
  THREADING_ENABLED ON # Required for shared memory
  TARGET_NAME "TypeLibrary"
  EXPORTED_FUNCTIONS "CreateMyStruct;GetValue;SetValue"
  SOURCE_FILES "side.c")

emscripten_module(
  MAIN_MODULE
  ES6_MODULE ON
  THREADING_ENABLED ON
  EMBIND ON
  EXPORT_NAME "loadES6Test"
  DEBUG "READABLE_JS"
  OPTIMIZATION "NONE"
  TARGET_NAME ${TARGET_NAME}
  SIDE_MODULES "TypeLibrary.wasm"
  SOURCE_FILES "ES6Test.cxx"
  JAVASCRIPT_FILES "run_ES6Test.js")

