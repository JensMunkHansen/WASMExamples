cmake_minimum_required(VERSION 3.15)
project(WASMExamples)

include(CTest)
enable_testing()

add_subdirectory(Embind/Template)
#add_subdirectory(Share/ShareViaHost)
add_subdirectory(Share/SharedHeap)

