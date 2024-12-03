cmake_minimum_required(VERSION 3.14)

project(RandomRWBuffer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WASM64 "64-bit WASM" OFF)

if(EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -msimd128")
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
    set_target_properties(random_rw PROPERTIES OUTPUT_NAME "random_rw.js")
    set_target_properties(random_rw PROPERTIES LINK_FLAGS "--bind")
    set_target_properties(random_lookup_dot PROPERTIES OUTPUT_NAME "random_lookup_dot.js")
    set_target_properties(random_lookup_dot PROPERTIES LINK_FLAGS "--bind")
    set_target_properties(random_lookup_dot_array PROPERTIES OUTPUT_NAME "random_lookup_dot_array.js")
    set_target_properties(random_lookup_dot_array PROPERTIES LINK_FLAGS "--bind")

    set(WASM_FILE "$<TARGET_FILE_DIR:random_lookup_dot_array>/$<TARGET_FILE_BASE_NAME:random_lookup_dot_array>.wasm")
    
    add_custom_command(
      TARGET random_lookup_dot_array POST_BUILD
      COMMAND wasm-objdump -d ${WASM_FILE} > ${CMAKE_BINARY_DIR}/random_lookup_dot_array.s
      COMMENT "Generating assembly output (.s) for WebAssembly"
    )    
else()
  # Custom target to generate assembly
  add_custom_target(assembly
    COMMAND ${CMAKE_CXX_COMPILER} -O3 -march=native -S ${CMAKE_CURRENT_SOURCE_DIR}/random_lookup_dot.cpp -o ${CMAKE_CURRENT_BINARY_DIR}/random_lookup_dot.s
    DEPENDS random_lookup_dot.cpp
    COMMENT "Generating assembly output for random_lookup_dot.cpp"
  )    
endif()


# make -f CMakeFiles/random_lookup_dot.dir/build.make CMakeFiles/random_lookup_dot.dir/random_lookup_dot.cpp.s
# wasm2wat random_lookup_dot_array.wasm > random_lookup_dot_array.wat
# wasm-objdump -d random_rw.js.wasm > dissambly.txt
