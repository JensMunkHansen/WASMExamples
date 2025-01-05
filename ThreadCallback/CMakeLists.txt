cmake_minimum_required(VERSION 3.22...3.28)

project(Catch2Test)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)
include(spsEmscriptenSettings)

sps_emscripten_module(
  ES6_MODULE OFF
  VERBOSE
  THREADING_ENABLED ON
  THREAD_POOL_SIZE 4
  TARGET_NAME main
  SOURCE_FILES main.cxx
  OPTIMIZATION NONE
  DEBUG PROFILE
)



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

