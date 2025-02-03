#include "emscripten/bind.h"
#include "side.h"
#include <iostream>

int AddOne(int k)
{
  struct MyStruct* r;
  CreateMyStruct(r);
  // Will print 0.0, since they do not share memory
  std::cout << "Main module: Accessing data: " << r->a << std::endl;
  // Even a return value cannot be shared (has to be via host)
  float retValue = GetValue(r);
  std::cout << "Main module: Calling GetValue: " << retValue << std::endl;

  // Memory has to be owned by main module to be shared
  struct MyStruct input;
  // Calling function in a shared library (setting variable to 2)
  SetValue(&input);
  std::cout << "Main module: After calling SetValue: " << input.a << std::endl;

  return k + 1;
}

EMSCRIPTEN_BINDINGS(ES6Test)
{
  emscripten::function("AddOne", &AddOne);
}
