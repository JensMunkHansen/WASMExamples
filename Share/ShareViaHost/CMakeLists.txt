cmake_minimum_required(VERSION 3.22)
project(ShareViaHost)

set(CMAKE_CXX_FLAGS "-s MODULARIZE=1 -s EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\"]'")
set(CMAKE_CXX_COMPILER em++)

add_executable(fill_array fill_array.cpp)
set(emscripten_link_options)
list(APPEND emscripten_link_options
  "-sEXPORTED_FUNCTIONS=['_malloc', '_free', '_fill_array']"
  "-sEXPORT_NAME='FILL_ARRAY_MODULE'")
target_link_options(fill_array
  PUBLIC
  ${emscripten_link_options})

add_executable(calc_sum calc_sum.cpp)
set(emscripten_link_options)
list(APPEND emscripten_link_options
  "-sEXPORTED_FUNCTIONS=['_malloc', '_free', '_calc_sum']"
  "-sEXPORT_NAME='CALC_SUM_MODULE'")
target_link_options(calc_sum
  PUBLIC
  ${emscripten_link_options})

add_custom_command(
  TARGET calc_sum
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/index.html"
    $<TARGET_FILE_DIR:calc_sum>)

