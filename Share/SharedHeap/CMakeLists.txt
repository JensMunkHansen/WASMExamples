cmake_minimum_required(VERSION 3.22)
project(SharedHeap)

#set(CMAKE_CXX_FLAGS )
set(CMAKE_CXX_COMPILER em++)

add_executable(fill_array_heap fill_array.cpp)
set(emscripten_link_options)
list(APPEND emscripten_link_options
  "-sEXPORTED_FUNCTIONS=['_fill_array']"
  "-sEXPORTED_RUNTIME_METHODS=[ccall, cwrap]"  
  "-sALLOW_MEMORY_GROWTH=1"
  "-sASSERTIONS=1" # This makes linker assert stuff so we can see what is wrong
  "-sIMPORTED_MEMORY=1"  
  "-sEXPORT_NAME=FILL_ARRAY_MODULE")
target_link_options(fill_array_heap
  PUBLIC
  ${emscripten_link_options})

add_executable(calc_sum_heap calc_sum.cpp)
set(emscripten_link_options)
list(APPEND emscripten_link_options
  "-sEXPORTED_FUNCTIONS=['_calc_sum']"
  "-sEXPORTED_RUNTIME_METHODS=[ccall, cwrap]"  
  "-sALLOW_MEMORY_GROWTH=1"
  "-sASSERTIONS=1" # This makes linker assert stuff so we can see what is wrong
  "-sIMPORTED_MEMORY=1"  
  "-sEXPORT_NAME=CALC_SUM_MODULE")
target_link_options(calc_sum_heap
  PUBLIC
  ${emscripten_link_options})

add_custom_command(
  TARGET calc_sum_heap
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/index.html"
    $<TARGET_FILE_DIR:calc_sum_heap>)

