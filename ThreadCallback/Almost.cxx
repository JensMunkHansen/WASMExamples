#include <chrono>
#include <emscripten/bind.h>
#include <emscripten/threading.h>
#include <iostream>
#include <pthread.h>
#include <thread>

// Global callback function
emscripten::val callbackFunction = emscripten::val::null();

// Function to set the callback
void setCallback(emscripten::val callback)
{
  if (callback.typeOf().as<std::string>() == "function")
  {
    callbackFunction = callback;
    std::cout << "Callback function set successfully." << std::endl;

    // Test the callback with a sample value
    try
    {
      int testValue = 42;
      int result = callbackFunction(testValue).as<int>();
      std::cout << "Callback test with value " << testValue << " returned: " << result << std::endl;
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error testing callback: " << e.what() << std::endl;
    }
  }
  else
  {
    std::cerr << "Invalid callback received. It is not a function!" << std::endl;
  }
}

// Wrapper function to call the callback on the main thread
void callCallbackOnMainThread(void* arg)
{
  int value = reinterpret_cast<intptr_t>(arg); // Safely cast the pointer to an integer

  if (!callbackFunction.isNull())
  {
    try
    {
      callbackFunction(value);
      std::cout << "Callback executed on main thread with value: " << value << std::endl;
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error calling callback on main thread: " << e.what() << std::endl;
    }
  }
  else
  {
    std::cerr << "Callback function is null or invalid on main thread!" << std::endl;
  }
}

// Thread function
void* threadFunction(void* arg)
{
  std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
  int value = 42;

  // Dispatch to the main thread
  emscripten_dispatch_to_thread(emscripten_main_browser_thread_id(), EM_FUNC_SIG_VI,
    callCallbackOnMainThread, reinterpret_cast<void*>(static_cast<intptr_t>(value)));
  return nullptr;
}

// Start pthread
void startPthread()
{
  pthread_t thread;
  if (pthread_create(&thread, nullptr, threadFunction, nullptr) != 0)
  {
    std::cerr << "Failed to create thread" << std::endl;
    return;
  }
  pthread_join(thread, nullptr);
}

// Embind bindings
EMSCRIPTEN_BINDINGS(callback_module)
{
  using namespace emscripten;

  // Bind the setCallback and startPthread functions
  function("setCallback", &setCallback);
  function("startPthread", &startPthread);
}
