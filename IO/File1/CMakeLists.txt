cmake_minimum_required(VERSION 3.12)
project(WASM_FileSystem_Test)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../CMake")
include(spsEmscriptenSettings)
include (CTest)
enable_testing()
# Add source files
sps_emscripten_module(
  TARGET_NAME example
  SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/example.cpp
  ES6_MODULE ON
  FILE_SYSTEM ON
  EXPORTED_FUNCTIONS TestMe;malloc;free)
target_include_directories(example PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Enable ExternalData for test files
include(ExternalData)
set(ExternalData_SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/test_files")
# 
# External files
set(input_file "${CMAKE_CURRENT_SOURCE_DIR}/test_files/input.json")
set(expected_output_file "${CMAKE_CURRENT_SOURCE_DIR}/test_files/expected_output.json")

ExternalData_add_test(
    NAME wasm_test
    COMMAND ${CMAKE_COMMAND} -E echo "Running WASM Test..."
    DATA
        ${input_file}
        ${expected_output_file}
)

# Add test runner
add_test(NAME Run_WASM_Test
    COMMAND node ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_wasm_test_file.js
        --wasm ${CMAKE_BINARY_DIR}/example.wasm
        --input ${input_file}
        --expected ${expected_output_file}
)

