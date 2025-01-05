#include <chrono>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <string>
#include <thread>

// Global callback stored as emscripten::val

// NOT GOOD. emscripten::val only allowed on main thread!!!
emscripten::val globalCallback = emscripten::val::null();

// Function to set the callback
void setCallback(emscripten::val callback)
{
  if (callback.typeOf().as<std::string>() == "function")
  {
    globalCallback = callback;
    std::cout << "Callback function set successfully." << std::endl;
  }
  else
  {
    std::cerr << "Invalid callback provided. Must be a JavaScript function." << std::endl;
  }
}

// Function to call the JavaScript callback
extern "C"
{
  EMSCRIPTEN_KEEPALIVE
  void invokeCallbackOnMainThread(void* msg)
  {
    std::cout << "Invoking" << std::endl;
    const char* message = static_cast<const char*>(msg);
    if (!globalCallback.isNull() && globalCallback.typeOf().as<std::string>() == "function")
    {
      globalCallback(std::string(message)); // Call the JavaScript callback
    }
    else
    {
      std::cerr << "No valid callback set!" << std::endl;
    }
  }
}
// Function executed by the pthread
void* pthreadWorker(void* arg)
{
  std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
  const char* result = "Hello from the pthread!";

  // Dispatch to the main thread to invoke the callback
  emscripten_async_call(
    reinterpret_cast<void (*)(void*)>(invokeCallbackOnMainThread), const_cast<char*>(result), 0);
  return nullptr;
}

// Starts a pthread
void startPthread()
{
  pthread_t thread;
  if (pthread_create(&thread, nullptr, pthreadWorker, nullptr) != 0)
  {
    std::cerr << "Failed to create pthread!" << std::endl;
  }
  else
  {
    std::cout << "Pthread started successfully!" << std::endl;

    // Detach the thread to avoid blocking the main thread
    if (pthread_detach(thread) != 0)
    {
      std::cerr << "Failed to detach pthread!" << std::endl;
    }
  }
}

// Bindings
EMSCRIPTEN_BINDINGS(my_module)
{
  using namespace emscripten;

  function("setCallback", &setCallback);
  function("startPthread", &startPthread);
}
