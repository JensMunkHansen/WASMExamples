#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int thread_started = 0;

void* thread_callback(void* arg)
{
  int* val = (int*)arg;

  // Signal that the worker thread has started
  pthread_mutex_lock(&mutex);
  thread_started = 1;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);

  *val += 1;
  printf("Worker thread: incremented value to %d\n", *val);
  return NULL;
}

// emrun example.html (works, emrun --no_browser --port 8080 .
int main()
{
  puts("Before the thread");

  pthread_t thread_id;
  int arg = 42;
  pthread_create(&thread_id, NULL, thread_callback, &arg);

  // Wait until the worker thread has started
  pthread_mutex_lock(&mutex);
  while (!thread_started)
  {
    pthread_cond_wait(&cond, &mutex);
  }
  pthread_mutex_unlock(&mutex);

  pthread_join(thread_id, NULL);

  puts("After the thread");

  return 0;
}
