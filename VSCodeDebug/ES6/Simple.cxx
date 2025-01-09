#include <iostream>

extern "C"
{
  void HelloWorld()
  {
    std::cout << "HelloWorld\n";
  }
}
