#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int thread1_started = 0;
int thread2_started = 0;

void* worker1(void* arg)
{
  int* val = (int*)arg;

  // Signal that thread 1 has started
  pthread_mutex_lock(&mutex);
  thread1_started = 1;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);

  // Do some work with synchronized access to shared variable
  pthread_mutex_lock(&mutex);
  *val += 1;
  printf("Worker 1: incremented value to %d\n", *val);
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void* worker2(void* arg)
{
  int* val = (int*)arg;

  // Signal that thread 2 has started
  pthread_mutex_lock(&mutex);
  thread2_started = 1;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);

  // Do some work with synchronized access to shared variable
  pthread_mutex_lock(&mutex);
  *val += 2;
  printf("Worker 2: incremented value to %d\n", *val);
  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main()
{
  pthread_t thread1, thread2;
  int value = 0;

  printf("Main thread: initial value is %d\n", value);

  // Create worker threads
  if (pthread_create(&thread1, NULL, worker1, &value) != 0)
  {
    printf("Error creating thread 1\n");
    return 1;
  }
  if (pthread_create(&thread2, NULL, worker2, &value) != 0)
  {
    printf("Error creating thread 2\n");
    return 1;
  }

  // Wait until both threads have started
  pthread_mutex_lock(&mutex);
  while (!thread1_started || !thread2_started)
  {
    pthread_cond_wait(&cond, &mutex);
  }
  pthread_mutex_unlock(&mutex);

  // Join both worker threads
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  printf("Main thread: final value is %d\n", value);
  return 0;
}
