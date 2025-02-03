#include <emscripten.h>
#include <emscripten/bind.h>

int MyFunction(int k)
{
  return k + 42;
}

EMSCRIPTEN_BINDINGS(ThreadingCPP)
{
  // Bind ConsumeTest and allow raw pointers
  emscripten::function("MyFunction", &MyFunction);
}
