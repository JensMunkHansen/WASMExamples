#include <iostream>
#include <thread>

void threadFunction()
{
  std::cout << "Hello from thread!" << std::endl;
}

int main()
{
  try
  {
    std::thread t(threadFunction);
    t.join();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
