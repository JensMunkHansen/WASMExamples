#include <chrono>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include <thread>

#include <condition_variable>
#include <mutex>

// Shared synchronization variables
std::condition_variable cv;
std::condition_variable async_cv;
std::mutex cv_mtx;
bool isThreadComplete = false;
bool isAsyncCallComplete = false;

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
      MAIN_THREAD_EM_ASM(
        {
          console.log("Executing async call in main thread...");
          Module.ccall('notifyCallback', null, ['string'], [UTF8ToString($0)]);
        },
        message);

      // Signal that the async call is complete
      {
        std::lock_guard<std::mutex> lock(cv_mtx);
        isAsyncCallComplete = true;
      }
      async_cv.notify_one();

      // Signal that the thread has completed
      {
        std::lock_guard<std::mutex> lock(cv_mtx);
        isThreadComplete = true;
      }
      cv.notify_one();
    },
    const_cast<char*>(result), 0);

  std::cout << "Async call scheduled." << std::endl;

  return nullptr;
}

// Starts a pthread
void startPthread()
{
  std::cout << "startPthread: " << std::this_thread::get_id() << std::endl;

  pthread_t thread;
  if (pthread_create(&thread, nullptr, pthreadWorker, nullptr) != 0)
  {
    std::cerr << "Failed to create pthread!" << std::endl;
  }
  else
  {
    std::cout << "Pthread started successfully!" << std::endl;

    if (pthread_detach(thread) != 0)
    {
      std::cerr << "Failed to detach pthread!" << std::endl;
    }
    else
    {
      std::cout << "Pthread detached successfully." << std::endl;
    }

    // Wait for the detached thread to signal completion
    {
      std::unique_lock<std::mutex> lock(cv_mtx);
      cv.wait(lock, [] { return isThreadComplete; });
    }
    std::cout << "Main thread: Worker thread has completed." << std::endl;

    // Wait for the async call to complete
    {
      std::unique_lock<std::mutex> lock(cv_mtx);
      async_cv.wait(lock, [] { return isAsyncCallComplete; });
    }
  }
}

// Expose a placeholder function for notifyCallback
extern "C"
{
  EMSCRIPTEN_KEEPALIVE
  void notifyCallback(const char* message)
  {
    std::cout << "notifyCallback: " << std::this_thread::get_id() << std::endl;
    std::cout << "notifyCallback called with message: " << message << std::endl;
  }
}

// Bindings
EMSCRIPTEN_BINDINGS(my_module)
{
  using namespace emscripten;
  function("startPthread", &startPthread);
}
