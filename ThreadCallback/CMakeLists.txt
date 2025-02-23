cmake_minimum_required(VERSION 3.22...3.28)

project(ThreadCallback)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)
include(spsEmscriptenDefaults)

if (NOT DEFINED WASMExamples_DEBUGINFO)
  # We anticipate all options are set
  sps_set_emscripten_defaults(${PROJECT_NAME})
endif()
include(spsEmscriptenSettings)

find_package(VTK
  COMPONENTS
    CommonCore
)
include(CTest)
enable_testing()

# C/C++ callback
sps_emscripten_module(
  ES6_MODULE OFF
  VERBOSE
  THREADING_ENABLED ON
  THREAD_POOL_SIZE 4
  TARGET_NAME SimpleCallback
  SOURCE_FILES SimpleCallback.cxx
  OPTIMIZATION ${${PROJECT_NAME}_OPTIMIZATION}
  DEBUG ${${PROJECT_NAME}_DEBUGINFO}
)

# VTK callback
sps_emscripten_module(
  ES6_MODULE OFF
  VERBOSE
  THREADING_ENABLED ON
  THREAD_POOL_SIZE 4
  TARGET_NAME VTKCallback
  LIBRARIES VTK::CommonCore
  SOURCE_FILES VTKCallback.cxx
  OPTIMIZATION ${${PROJECT_NAME}_OPTIMIZATION}
  DEBUG ${${PROJECT_NAME}_DEBUGINFO}
)

# JavaScript callback (same thread)
sps_emscripten_module(
  ES6_MODULE ON
  VERBOSE
  THREADING_ENABLED ON
  THREAD_POOL_SIZE 4
  TARGET_NAME JSCallbackTest
  EXPORT_NAME loadJSCallbackTestModule
  JAVASCRIPT_FILES run_JSCallbackTest.js
  EXPORTED_FUNCTIONS invokeCallback
  INITIAL_MEMORY 1GB
  SOURCE_FILES JSCallback.cxx
  EMBIND ON
  OPTIMIZATION ${${PROJECT_NAME}_OPTIMIZATION}
  DEBUG ${${PROJECT_NAME}_DEBUGINFO}
)

add_test(NAME ThreadCallback_JSInvoke_NoErrors
  COMMAND
    ${CMAKE_COMMAND} -E env
    "CMAKE_CONFIG_TYPE=$<CONFIG>"  
    npx node run_JSCallbackTest.js
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

sps_emscripten_module(
  ES6_MODULE ON
  VERBOSE
  THREADING_ENABLED ON
  THREAD_POOL_SIZE 4
  TARGET_NAME JSCallbackThreadDetachedTest
  EXPORT_NAME loadJSCallbackDetachedTestModule
  EXPORTED_FUNCTIONS notifyCallback
  JAVASCRIPT_FILES run_JSCallbackThreadDetachedTest.js
  INITIAL_MEMORY 1GB
  ASYNCIFY ON
  SOURCE_FILES JSCallbackThreadDetached.cxx
  EMBIND ON
  OPTIMIZATION ${${PROJECT_NAME}_OPTIMIZATION}
  DEBUG ${${PROJECT_NAME}_DEBUGINFO}
)

add_test(NAME ThreadCallback_AsynDetached_NoErrors
  COMMAND
    ${CMAKE_COMMAND} -E env
    "CMAKE_CONFIG_TYPE=$<CONFIG>"  
    npx node run_JSCallbackThreadDetachedTest.js
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

sps_emscripten_module(
  ES6_MODULE ON
  VERBOSE
  THREADING_ENABLED ON
  THREAD_POOL_SIZE 4
  TARGET_NAME ES6CallbackTest
  JAVASCRIPT_FILES run_ES6CallbackTest.js
  EXPORT_NAME loadES6CallbackTestModule
  INITIAL_MEMORY 1GB
  SOURCE_FILES ES6Callback.cxx
  EMBIND ON
  OPTIMIZATION ${${PROJECT_NAME}_OPTIMIZATION}
  DEBUG ${${PROJECT_NAME}_DEBUGINFO}
)

add_test(NAME ES6CallbackTest_NoErrors
  COMMAND
    ${CMAKE_COMMAND} -E env
    "CMAKE_CONFIG_TYPE=$<CONFIG>"  
    npx node run_ES6CallbackTest.js
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

# Next Steps Now that the basic threading and callback mechanism is
# working, you can expand this example:
# 
# 1. Integrate with VTK:
# 
# Replace the plain-old C function with a vtkCallbackCommand
# callback, and test invoking events in a multithreaded context.
# 
# 2. Test with WebAssembly:
# 
#  If you’re targeting WebAssembly, ensure that all the necessary
#  Emscripten options for PThreads and shared memory are properly
#  configured.
# 
# 3. Pass More Complex Data:
# 
# Extend the callback to accept and process more complex data (e.g.,
# structs or dynamically allocated memory). Make sure to handle memory
# management properly.
# 
# 4. Thread Pool:
# 
# Experiment with creating multiple PThreads to invoke callbacks concurrently.
# 
# 5. Error Handling:
# 
# Add error handling to gracefully manage issues like thread creation
# failures or invalid arguments to the callback.

