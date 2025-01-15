cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)

# Project name
project(EightQueens
  LANGUAGES C CXX)

include(spsEmscriptenSettings)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(ENABLE_EXCEPTIONS OFF)

set(TARGET_NAME eight_queens)
if (EMSCRIPTEN)
  sps_emscripten_module(
    ES6_MODULE OFF
    ENABLE_EXCEPTIONS ${ENABLE_EXCEPTIONS}
    SOURCE_FILES EightQueen.cxx
    TARGET_NAME ${TARGET_NAME}
    DEBUG NONE
    OPTIMIZATION BEST
  )
else()
  # Add the main executable
  add_executable(${TARGET_NAME} EightQueen.cxx)
  if (NOT ${ENABLE_EXCEPTIONS})
    # Disable exceptions for the target
    target_compile_options(${TARGET_NAME} PRIVATE
      $<$<CXX_COMPILER_ID:GNU>:-fno-exceptions>
      $<$<CXX_COMPILER_ID:Clang>:-fno-exceptions>
      $<$<CXX_COMPILER_ID:MSVC>:/EHs->
    )
  endif()
endif()

# Enable testing
include(CTest)

# Generate the expected .wasm output filename
set(WASM_FILE "${CMAKE_CURRENT_BINARY_DIR}/eight_queens.wasm")

# Add a custom test that executes the main program
add_test(
    NAME EightQueensTest
    COMMAND ${TARGET_NAME}
)


# Test to verify the .wasm file size
add_test(
  NAME VerifyWasmSize
  COMMAND ${CMAKE_COMMAND} -E env sh -c "[ $(stat -c%s ${WASM_FILE}) -lt 524288 ] || (echo 'File too large!' && exit 1)"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Custom target to run all tests
add_custom_target(
    check_wasm_size
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
)
