cmake_minimum_required(VERSION 3.22)
project(SharedHeap)

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_CXX_FLAGS "-s MODULARIZE=1 -s EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\"]' -s IMPORTED_MEMORY=1")
set(CMAKE_CXX_COMPILER em++)

add_executable(fill_array.js fill_array.cpp)
set_target_properties(fill_array.js 
    PROPERTIES LINK_FLAGS "-s EXPORTED_FUNCTIONS='[\"_fill_array\"]' -s EXPORT_NAME='FILL_ARRAY_MODULE'"
    )

add_executable(calc_sum.js calc_sum.cpp)
set_target_properties(calc_sum.js 
    PROPERTIES LINK_FLAGS "-s EXPORTED_FUNCTIONS='[\"_calc_sum\"]' -s EXPORT_NAME='CALC_SUM_MODULE'")

add_custom_command(
  TARGET calc_sum.js
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/index.html"
    $<TARGET_FILE_DIR:calc_sum.js>)

