#include <iostream>
#include <mutex>
#include <pthread.h>
#include <thread>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkObject.h>
#include <vtkSmartPointer.h>

// Mutex for thread-safe logging
std::mutex logMutex;

// Callback function
void CallbackFunction(vtkObject* caller, unsigned long eventId, void* clientData, void* callData)
{
  std::lock_guard<std::mutex> lock(logMutex);
  std::cout << "Callback executed in thread: " << pthread_self() << ", Event ID: " << eventId
            << ", Message: " << static_cast<const char*>(callData) << std::endl;
}

// Pthread function to invoke events
void* ThreadFunction(void* arg)
{
  vtkObject* vtkObjectInstance = static_cast<vtkObject*>(arg);

  const char* message = "Hello from pthread!";
  for (int i = 0; i < 5; ++i)
  {
    vtkObjectInstance->InvokeEvent(vtkCommand::UserEvent, (void*)message);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  return nullptr;
}

int main()
{
  // Create a VTK object
  auto vtkObjectInstance = vtkSmartPointer<vtkObject>::New();

  // Create a callback command
  auto callback = vtkSmartPointer<vtkCallbackCommand>::New();
  callback->SetCallback(CallbackFunction);

  // Add the callback as an observer
  vtkObjectInstance->AddObserver(vtkCommand::UserEvent, callback);

  // Create and start a pthread
  pthread_t thread;
  pthread_create(&thread, nullptr, ThreadFunction, vtkObjectInstance);

  // Trigger an event from the main thread
  const char* mainThreadMessage = "Hello from the main thread!";
  vtkObjectInstance->InvokeEvent(vtkCommand::UserEvent, (void*)mainThreadMessage);

  // Wait for the pthread to finish
  pthread_join(thread, nullptr);

  return 0;
}
