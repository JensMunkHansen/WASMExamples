cmake_minimum_required(VERSION 3.22...3.28)

project(ThreadCallback)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../CMake)
include(spsEmscriptenSettings)
sps_set_emscripten_defaults(${PROJECT_NAME})

include(CTest)
enable_testing()

sps_emscripten_module(
  ES6_MODULE ON
  VERBOSE
  THREADING_ENABLED ON
  THREAD_POOL_SIZE 4
  TARGET_NAME JSCallbackThreadJoinableTest
  EXPORT_NAME loadJSCallbackTestModule
  JAVASCRIPT_FILES run_JSCallbackThreadJoinableTest.js
  INITIAL_MEMORY 1GB
  ASYNCIFY OFF
  SOURCE_FILES JSCallbackThreadJoinable.cxx # JSCallbackThread.cxx (works)
  EMBIND ON
  OPTIMIZATION ${${PROJECT_NAME}_OPTIMIZATION}
  DEBUG ${${PROJECT_NAME}_DEBUGINFO}
)

add_test(NAME ThreadCallback_AsynJoinable_NoErrors
  COMMAND
  npx node run_JSCallbackThreadJoinableTest.js
  WORKING_DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR})


