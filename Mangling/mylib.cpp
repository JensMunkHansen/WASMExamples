#include <iostream>

extern "C" void __attribute__((visibility("default"))) print_message()
{
  std::cout << "Hello from WebAssembly!" << std::endl;
}

// A mangled C++ function
int mangled_function(int x)
{
  return x * x;
}
