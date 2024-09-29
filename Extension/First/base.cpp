#include "base.h"
#include <iostream>

void BaseClass::greet()
{
  std::cout << "Hello from BaseClass!" << std::endl;
}

OtherClass::OtherClass()
{
  f = 2.0f;
}
