cmake_minimum_required(VERSION 3.21)
project(hello)

add_executable(hello hello.c)
target_include_directories(hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# DWARF only - no symbol maps -gdwarf
#set(emscripten_optimization_flags  -O0 -gdwarf)
#set(emscripten_optimization_flags -O0 -g3)

# With source map, symlink /emsdk to match the upstream folder in Emscripten.
set(emscripten_optimization_flags  -O0 -gsource-map)
# Only works, if MODULARIZE=0 and no ES6
target_link_options(hello PUBLIC
  "-sUSE_WEBGL2=1"
  #  "-sMODULARIZE=1"
  "-sASYNCIFY=1" # Test to enter Emscripten C++ code
  "--shell-file=../shell.html"  
#  "-sEXPORT_ES6=1"  
#  "-sEXPORTED_RUNTIME_METHODS=\"ccall\", \"cwrap\", \"addFunction\""
  "-sEXPORTED_FUNCTIONS=['_free', '_malloc', '_main']"
  "-sINCLUDE_FULL_LIBRARY=1"
  "-sALLOW_TABLE_GROWTH=1"  
#  "-sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE='$addFunction'"  # Check if needed if full
  "${emscripten_optimization_flags}")
# target_compile_options(hello PUBLIC --emit-symbol-map)
if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()
