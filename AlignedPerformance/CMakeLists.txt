cmake_minimum_required(VERSION 3.22...3.28)

project(AlignedPerformance)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)

# TODO: Fix generic build system
set(USE_NEW FALSE)

option(WASM64 "64-bit WASM" OFF)
option(SIMD OFF)

# TODO: Do this on target instead
if(${WASM64} STREQUAL "ON")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --target=wasm64")
endif()


include(spsEmscriptenDefaults)

if (NOT DEFINED WASMExamples_DEBUG)
  # We anticipate all options are set
  sps_set_emscripten_defaults(WASMExamples)
endif()
include(spsEmscriptenSettings)

if (USE_NEW)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(EMSCRIPTEN)
  sps_emscripten_module(
    64_BIT ${WASM64}
    ES6_MODULE OFF
    TARGET_NAME random_rw
    SOURCE_FILES random_rw.cpp
    DEBUG ${WASMExamples_DEBUG}
    OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  )
  sps_emscripten_module(
    64_BIT ${WASM64}
    ES6_MODULE OFF
    TARGET_NAME random_lookup_dot
    SOURCE_FILES random_lookup_dot.cpp
    DEBUG ${WASMExamples_DEBUG}
    OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  )
  sps_emscripten_module(
    VERBOSE
    64_BIT ${WASM64}
    ES6_MODULE OFF
    TARGET_NAME random_lookup_dot_array
    SOURCE_FILES random_lookup_dot_array.cpp
    DEBUG ${WASMExamples_DEBUG}
    OPTIMIZATION ${WASMExamples_OPTIMIZATION}
  )

  set(WASM_FILE "$<TARGET_FILE_DIR:random_lookup_dot_array>/$<TARGET_FILE_BASE_NAME:random_lookup_dot_array>.wasm")

  if (NOT WASM64)
    if (CMAKE_CONFIGURATION_TYPES)
      add_custom_command(
        TARGET random_lookup_dot_array POST_BUILD
        COMMAND wasm-objdump -d ${WASM_FILE} > $<CONFIG>/random_lookup_dot_array.s
        COMMENT "Generating assembly output (.s) for WebAssembly"
      )
    else()
      add_custom_command(
        TARGET random_lookup_dot_array POST_BUILD
        COMMAND wasm-objdump -d ${WASM_FILE} > ${CMAKE_CURRENT_BINARY_DIR}/random_lookup_dot_array.s
        COMMENT "Generating assembly output (.s) for WebAssembly"
      )
    endif()
  endif()
endif()




else()
  
  # Old project ejecting assembly
if(EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  if (SIMD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msimd128")
  endif()
  if (WASM64)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --target=wasm64")
  endif()
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --bind")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
endif()

add_executable(random_rw random_rw.cpp)
add_executable(random_lookup_dot random_lookup_dot.cpp)

add_executable(random_lookup_dot_array random_lookup_dot_array.cpp)


# Target for WASM (WebAssembly 32-bit)
if(EMSCRIPTEN)
    set_target_properties(random_rw PROPERTIES OUTPUT_NAME "random_rw")
    set_target_properties(random_rw PROPERTIES LINK_FLAGS "--bind")
    set_target_properties(random_lookup_dot PROPERTIES OUTPUT_NAME "random_lookup_dot")
    set_target_properties(random_lookup_dot PROPERTIES LINK_FLAGS "--bind")
    set_target_properties(random_lookup_dot_array PROPERTIES OUTPUT_NAME "random_lookup_dot_array")
    set_target_properties(random_lookup_dot_array PROPERTIES LINK_FLAGS "--bind")

    _sps_target_info(random_lookup_dot_array)
    
    set(WASM_FILE "$<TARGET_FILE_DIR:random_lookup_dot_array>/$<TARGET_FILE_BASE_NAME:random_lookup_dot_array>.wasm")

    if (NOT WASM64)
      add_custom_command(
        TARGET random_lookup_dot_array POST_BUILD
        COMMAND wasm-objdump -d ${WASM_FILE} > ${CMAKE_BINARY_DIR}/random_lookup_dot_array.s
        COMMENT "Generating assembly output (.s) for WebAssembly"
      )
    endif()
else()
  # Custom target to generate assembly
  add_custom_target(assembly
    COMMAND ${CMAKE_CXX_COMPILER} -O3 -march=native -S ${CMAKE_CURRENT_SOURCE_DIR}/random_lookup_dot.cpp -o ${CMAKE_CURRENT_BINARY_DIR}/random_lookup_dot.s
    DEPENDS random_lookup_dot.cpp
    COMMENT "Generating assembly output for random_lookup_dot.cpp"
  )    
endif()
endif()

# make -f CMakeFiles/random_lookup_dot.dir/build.make CMakeFiles/random_lookup_dot.dir/random_lookup_dot.cpp.s
# wasm2wat random_lookup_dot_array.wasm > random_lookup_dot_array.wat
# wasm-objdump -d random_rw.js.wasm > dissambly.txt
