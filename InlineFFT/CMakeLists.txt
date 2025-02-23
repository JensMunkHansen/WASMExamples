cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)

# Project name
project(FFT
  LANGUAGES C CXX)

include(spsEmscriptenDefaults)

if (NOT DEFINED WASMExamples_DEBUGINFO)
  # We anticipate all options are set
  sps_set_emscripten_defaults(WASMExamples)
endif()

include(spsEmscriptenSettings)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(ENABLE_EXCEPTIONS OFF)

set(TARGET_NAME fft)
if (EMSCRIPTEN)
  sps_emscripten_module(
    ES6_MODULE OFF
    ENABLE_EXCEPTIONS ${ENABLE_EXCEPTIONS}
    SOURCE_FILES fft.cxx
    TARGET_NAME ${TARGET_NAME}
    DEBUG ${WASMExamples_DEBUGINFO}
    OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  )
else()
  # Add the main executable
  add_executable(${TARGET_NAME} fft.cxx)
  if (NOT ${ENABLE_EXCEPTIONS})
    # Disable exceptions for the target
    target_compile_options(${TARGET_NAME} PRIVATE
      $<$<CXX_COMPILER_ID:GNU>:-fno-exceptions>
      $<$<CXX_COMPILER_ID:Clang>:-fno-exceptions>
      $<$<CXX_COMPILER_ID:MSVC>:/EHs->)

    target_compile_options(${TARGET_NAME} PRIVATE -ftemplate-depth=2048)    
  endif()
endif()

