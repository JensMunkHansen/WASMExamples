#include <chrono>
#include <emscripten.h>
#include <iostream>
#include <pthread.h>
#include <thread>

using MyCallbackType = void (*)(const char*);

MyCallbackType g_callback;

// Plain-old C function callback
extern "C"
{
  EMSCRIPTEN_KEEPALIVE
  void MyCallback(const char* message)
  {
    printf("Callback executed in thread: %lu, Message: %s\n", pthread_self(), message);
  }
}

// Function to register the callback
extern "C"
{
  EMSCRIPTEN_KEEPALIVE
  void RegisterCallback()
  {
    printf("Callback registered.\n");
  }
}

void RegisterRuntimeCallback(MyCallbackType callback)
{
  g_callback = callback;
}

// Pthread function to invoke the callback
void* ThreadFunction(void* arg)
{
  const char* message = static_cast<const char*>(arg);

  // Call the registered callback
  MyCallback(message);

  g_callback(message);
  return nullptr;
}

int main()
{
  // Register the callback during startup
  RegisterCallback();

  MyCallbackType myCallback = [](const char* message)
  {
    std::cout << "Runtime callback executed in thread: " << pthread_self()
              << ", Message: " << message << std::endl;
  };
  RegisterRuntimeCallback(myCallback);

  // Create a pthread
  pthread_t thread;
  const char* message = "Hello from PThread!";
  pthread_create(&thread, nullptr, ThreadFunction, (void*)message);

  // Wait for the pthread to finish
  pthread_join(thread, nullptr);

  // Call the callback from the main thread
  MyCallback("Hello from the main thread!");

  return 0;
}
