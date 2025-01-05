#include <chrono>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <functional>
#include <iostream>
#include <pthread.h>
#include <thread>

// Global callback function

// Function to set the callback
std::function<int(int)> callbackFunction = nullptr;

void setCallback(const std::function<int(int)>& callback)
{
  callbackFunction = callback;

  if (callbackFunction)
  {
    std::cout << "Callback function set successfully." << std::endl;

    // Test the callback with a sample value
    int testValue = 42;
    std::cout << "Testing callback with " << testValue << ": " << callbackFunction(testValue)
              << std::endl;
  }
  else
  {
    std::cerr << "Callback function set to nullptr!" << std::endl;
    // TODO: Figure this out.
    callbackFunction = [](int x) { return x * 2; };
  }
}

// Thread function
void* threadFunction(void* arg)
{
  if (callbackFunction)
  {
    // Wait for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Call the callback with a sample value (e.g., 42)
    int result = callbackFunction(42);
    std::cout << "Callback returned: " << result << std::endl;
  }
  else
  {
    std::cerr << "Callback is not set!" << std::endl;
  }

  return nullptr;
}

// Start a pthread
void startPthread()
{
  pthread_t thread;

  // Create the thread
  if (pthread_create(&thread, nullptr, threadFunction, nullptr) != 0)
  {
    std::cerr << "Failed to create thread" << std::endl;
    return;
  }

  // Join the thread
  if (pthread_join(thread, nullptr) != 0)
  {
    std::cerr << "Failed to join thread" << std::endl;
  }
}

// Embind bindings
void setCallbackWrapper(std::function<int(int)> callback)
{
  if (callback)
  {
    std::cout << "JavaScript callback received. Testing it now..." << std::endl;

    // Test the callback with a sample value
    int testValue = 42;
    try
    {
      int result = callback(testValue);
      std::cout << "Callback test with " << testValue << " returned: " << result << std::endl;

      // Assign to global callback
      callbackFunction = callback;
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error while testing callback: " << e.what() << std::endl;
    }
  }
  else
  {
    std::cerr << "JavaScript callback is nullptr!" << std::endl;
  }
}

// Binding code
EMSCRIPTEN_BINDINGS(callback_module)
{
  emscripten::value_object<std::function<int(int)>>("std::function<int(int)>");
  emscripten::function("setCallback", &setCallback);

  emscripten::function("startPthread", &startPthread);
}
