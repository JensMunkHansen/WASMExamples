#include <chrono>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include <thread>

// Shared state for thread synchronization
bool isThreadComplete = false;

// Function executed by the pthread
void* pthreadWorker(void* arg)
{
  std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
  const char* result = "Hello from the pthread!";
  std::cout << "Pthread finished work, scheduling async call." << std::endl;

  // Notify JavaScript via Module.ccall
  emscripten_async_call(
    [](void* msg)
    {
      const char* message = static_cast<const char*>(msg);
      EM_ASM(
        {
          console.log("Executing async call in main thread...");
          Module.ccall('notifyCallback', null, ['string'], [UTF8ToString($0)]);
        },
        message);
    },
    const_cast<char*>(result), 0);

  std::cout << "Async call scheduled." << std::endl;

  // Mark thread as complete
  isThreadComplete = true;
  return nullptr;
}

// Starts a pthread and waits for it to complete using pthread_join
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

    // Wait for the worker thread to complete
    pthread_join(thread, nullptr);
    std::cout << "Pthread joined successfully." << std::endl;
  }
}

// Expose a placeholder function for notifyCallback
extern "C"
{
  EMSCRIPTEN_KEEPALIVE
  void notifyCallback(const char* message)
  {
    std::cout << "notifyCallback called with message: " << message << std::endl;
  }
}

// Bindings
EMSCRIPTEN_BINDINGS(my_module)
{
  using namespace emscripten;
  function("startPthread", &startPthread);
}
