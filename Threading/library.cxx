#include "library.h"
#include "config.h"

#include <cstdlib>
#include <emscripten.h>
#include <emscripten/threading.h>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <thread>
#include <pthread.h>

#if _THREADING_ENABLED
namespace
{
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int threads_started = 0;

void* worker1(void* arg)
{
  int* val = (int*)arg;

  // Signal that worker 1 has started
  pthread_mutex_lock(&mutex);
  threads_started++;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);

  // Do some work
  pthread_mutex_lock(&mutex);
  *val += 1;

  std::ostringstream oss;
  oss << "Worker 1: incremented value to " << *val << "\n";
  std::cout << oss.str();
  pthread_mutex_unlock(&mutex);

#if 0
  // EMSCRIPTEN does not support naming pthreads!!!!
  std::thread::id thread_id = std::this_thread::get_id();

  // Convert thread ID to string
  std::ostringstream oss2;
  oss2 << thread_id;
  std::string id_str = oss2.str();

  // Ensure the name fits within the POSIX limit (16 bytes including null terminator)
  if (id_str.length() > 15) {
    id_str = id_str.substr(0, 15); // Truncate to fit the limit
  }
  // Set the thread name
  pthread_setname_np(pthread_self(), id_str.c_str());

  char thread_name[16];
  pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
  std::cout << thread_name << std::endl;
#endif
  return NULL;
}

void* worker2(void* arg)
{
  int* val = (int*)arg;

  // Signal that worker 2 has started
  pthread_mutex_lock(&mutex);
  threads_started++;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);

  // Do some work
  pthread_mutex_lock(&mutex);
  *val += 2;
  std::ostringstream oss;
  oss << "Worker 2: incremented value to " << *val << "\n";
  std::cout << oss.str();
  pthread_mutex_unlock(&mutex);

  return NULL;
}
} // namespace
#endif

#if 0
if (typeof Module !== 'undefined' && Module.unusedWorkers) {
        return Module.unusedWorkers.length;
    } else {
        return 0;
 }
#endif

int DoWork()
{
  EM_ASM({ console.log('hello'); });
  int k = EM_ASM_INT({ return PThread.unusedWorkers.length });
  std::cout << "Unused workers: " << k << std::endl;

  std::ostringstream oss;
  size_t nThreads = emscripten_num_logical_cores();
  oss << "Starting: DoWork(): available threads: " << nThreads << "\n";
  std::cout << oss.str();
  oss.clear();

#if _THREADING_ENABLED
  pthread_t thread1, thread2;
  int value = 0;

  oss << "Main thread: initial value is " << value << "\n";
  std::cout << oss.str();
  oss.clear();

  // Create worker threads
  if (pthread_create(&thread1, NULL, worker1, &value) != 0)
  {
    oss << "Error creating thread 1\n";
    std::cout << oss.str();
    oss.clear();
    return EXIT_FAILURE;
  }
  
  if (pthread_create(&thread2, NULL, worker2, &value) != 0)
  {
    oss << "Error creating thread 2\n";
    std::cout << oss.str();
    oss.clear();
    return EXIT_FAILURE;
  }

  // Wait until both threads have started
  pthread_mutex_lock(&mutex);
  while (threads_started < 2)
  {
    pthread_cond_wait(&cond, &mutex);
  }
  pthread_mutex_unlock(&mutex);

  // Join both worker threads
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  printf("Main thread: final value is %d\n", value);
#endif

  return EXIT_SUCCESS;
}
