cmake_minimum_required(VERSION 3.15)
project(WASMExamples)

include(CTest)
enable_testing()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
include(spsEmscriptenSettings)
include(spsEmscriptenDefaults)
sps_set_emscripten_defaults(${PROJECT_NAME})

add_subdirectory(Embind/Template)
#add_subdirectory(Share/ShareViaHost)
add_subdirectory(Share/SharedHeap)
add_subdirectory(ThreadCallback)
add_subdirectory(AlignedPerformance)
