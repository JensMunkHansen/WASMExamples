cmake_minimum_required(VERSION 3.21)
project(hello)
add_executable(hello hello.c)
if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX .js)
endif()
  
