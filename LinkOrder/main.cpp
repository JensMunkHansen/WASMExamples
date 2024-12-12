#include "factory_and_base.h"
#include <iostream>

int main()
{
  auto obj = Factory::create("Derived");
  obj->doSomething();
  return 0;
}
