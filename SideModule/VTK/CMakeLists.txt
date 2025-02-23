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
  CommonCore
)
set(BUILD_SHARED_LIBS ON)

set(TARGET_NAME VTKABITest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../CMake)
include(spsEmscriptenDefaults)

if (NOT DEFINED WASMExamples_DEBUGINFO)
  # We anticipate all options are set
  sps_set_emscripten_defaults(WASMExamples)
endif()
include(spsEmscriptenSettings)
include(spsExportHeader)

# We get linker errors if this is C++

# WebAssembly SIDE_MODULEs cannot directly export C++ types to be used in another WASM module.
#
# This limitation stems from how WebAssembly is designed and how C++
# works at a low level. WebAssembly focuses on low-level constructs,
# and exporting complex C++ types directly (e.g., classes, structs, or
# templates) between modules would require sharing both type layouts
# and implementations, which WebAssembly does not natively support.

set(SIDE_TARGET_NAME SideModule)
set(LIB_TYPE STATIC)
#set(LIB_TYPE SHARED)

if (EMSCRIPTEN)
sps_emscripten_module(
  SIDE_MODULE
  VERBOSE
  ES6_MODULE OFF
  THREADING_ENABLED ON # Required for shared memory
  TARGET_NAME ${SIDE_TARGET_NAME}
  EXPORTED_FUNCTIONS "Process"
  DEBUG ${WASMExamples_DEBUGINFO}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  SOURCE_FILES "side_array_abi.cxx")
else()
  add_executable(main main.cxx)
  target_link_libraries(main ${VTK_LIBRARIES})
  add_library(${SIDE_TARGET_NAME} ${LIB_TYPE} side_array_abi.cxx)
endif()
target_include_directories(${SIDE_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>>")

if (EMSCRIPTEN)
sps_emscripten_module(
  SIDE_MODULE
  VERBOSE
  ES6_MODULE OFF
  THREADING_ENABLED ON # Required for shared memory
  TARGET_NAME Runtime
  EXPORTED_FUNCTIONS "@${CMAKE_CURRENT_SOURCE_DIR}/array_abi.json"
  DEBUG ${WASMExamples_DEBUGINFO}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  SOURCE_FILES "array_abi.cxx")
else()
  add_library(Runtime ${LIB_TYPE} array_abi.cxx)
endif()
sps_generate_custom_export_header(Runtime)

if (EMSCRIPTEN)
sps_emscripten_module(
  MAIN_MODULE
  VERBOSE
  ES6_MODULE ON
  THREADING_ENABLED ON
  EMBIND ON
  EXPORT_NAME "loadSideModuleTest"
  DEBUG ${WASMExamples_DEBUGINFO}
  OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  LIBRARIES ${VTK_LIBRARIES}
  TARGET_NAME ${TARGET_NAME}
  SIDE_MODULES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/SideModule.wasm;${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/Runtime.wasm"
  SOURCE_FILES "host_array.cxx"
  JAVASCRIPT_FILES "run_SideModuleTest.js")
else()
  add_executable(${TARGET_NAME} host_array.cxx)
  target_link_libraries(${TARGET_NAME} PRIVATE ${SIDE_TARGET_NAME} Runtime ${VTK_LIBRARIES})
endif()
target_include_directories(${TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>>")

add_dependencies(${TARGET_NAME} ${SIDE_TARGET_NAME})
add_dependencies(${SIDE_TARGET_NAME} Runtime)

add_test(NAME ES6_SideModule_NoErrors
  COMMAND
  npx node run_SideModuleTest.js
  WORKING_DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR})
