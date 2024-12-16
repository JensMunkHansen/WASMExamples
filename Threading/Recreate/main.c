#include <emscripten/threading.h>
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 4

void* worker_function(void* arg)
{
  int thread_id = *(int*)arg;
  printf("Thread %d: Starting work.\n", thread_id);

  // Simulate work
  emscripten_thread_sleep(1000);

  printf("Thread %d: Work complete.\n", thread_id);
  return NULL;
}

int main()
{
  pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];

  for (int round = 0; round < 2; ++round)
  { // Two rounds of thread execution
    printf("Round %d: Creating threads...\n", round + 1);

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
      thread_ids[i] = i;
      if (pthread_create(&threads[i], NULL, worker_function, &thread_ids[i]) != 0)
      {
        perror("Error creating thread");
        return 1;
      }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
      if (pthread_join(threads[i], NULL) != 0)
      {
        perror("Error joining thread");
        return 1;
      }
    }

    printf("Round %d: All threads joined.\n", round + 1);
  }

  printf("All rounds complete.\n");
  return 0;
}
