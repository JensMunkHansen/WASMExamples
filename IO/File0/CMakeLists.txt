cmake_minimum_required(VERSION 3.12)
project(WASM_Test_Example
LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../CMake")
include(spsEmscriptenSettings)
include (CTest)
enable_testing()
# Add source files
sps_emscripten_module(
  TARGET_NAME example
  SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/example.cpp
  ES6_MODULE ON
  EXPORTED_FUNCTIONS sum_array;malloc;free)

# Enable ExternalData for test files
include(ExternalData)
set(ExternalData_SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/test_files")

# External files
set(input_file "${CMAKE_CURRENT_SOURCE_DIR}/test_files/input.json")
set(expected_output_file "${CMAKE_CURRENT_SOURCE_DIR}/test_files/expected_output.json")

# TODO: Use ExternalData_Add_Target
# add_dependencies(Run_WASM_Test WasmTestData)
      
if (1)
  # Add test runner
  add_test(NAME Run_WASM_Test
    COMMAND
      npx node ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_wasm_test.js
      --wasm ${CMAKE_CURRENT_BINARY_DIR}/example.wasm
      --input ${CMAKE_CURRENT_SOURCE_DIR}/test_files/input.json
      --expected ${CMAKE_CURRENT_SOURCE_DIR}/test_files/expected_output.json
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR})
endif()
