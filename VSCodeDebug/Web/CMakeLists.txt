cmake_minimum_required(VERSION 3.21)
project(hello)

add_executable(hello hello.c)
target_include_directories(hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set(emscripten_optimization_flags -g3 -O0)
# Only works, if MODULARIZE=0 and no ES6
target_link_options(hello PUBLIC
  "-sUSE_WEBGL2=1"
#  "-sMODULARIZE=1"
  "--shell-file=../shell.html"  
#  "-sEXPORT_ES6=1"  
#  "-sEXPORTED_RUNTIME_METHODS=\"ccall\", \"cwrap\", \"addFunction\""
  "-sEXPORTED_FUNCTIONS=['_free', '_malloc', '_main']"
  "-sINCLUDE_FULL_LIBRARY=1"
  "-sALLOW_TABLE_GROWTH=1"  
#  "-sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE='$addFunction'"  # Check if needed if full
  "${emscripten_optimization_flags}")

if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()
