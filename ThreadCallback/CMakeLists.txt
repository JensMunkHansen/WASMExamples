cmake_minimum_required(VERSION 3.22...3.28)

project(Catch2Test)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)
include(spsEmscriptenSettings)

find_package(VTK
  COMPONENTS
    CommonCore
)

# C/C++ callback
sps_emscripten_module(
  ES6_MODULE OFF
  VERBOSE
  THREADING_ENABLED ON
  THREAD_POOL_SIZE 4
  TARGET_NAME SimpleCallback
  SOURCE_FILES SimpleCallback.cxx
  OPTIMIZATION NONE
  DEBUG PROFILE
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
  OPTIMIZATION NONE
  DEBUG PROFILE
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
  OPTIMIZATION NONE
  DEBUG PROFILE
)

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
  OPTIMIZATION NONE
  DEBUG PROFILE
)
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
  SOURCE_FILES JSCallbackThreadDetached.cxx # JSCallbackThread.cxx (works)
  EMBIND ON
  OPTIMIZATION NONE
  DEBUG PROFILE
)


# Not working
if (1)
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
  OPTIMIZATION NONE
  DEBUG PROFILE
)
endif()






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

