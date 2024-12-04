#include "emscripten/bind.h"
#include "side.h"
int AddOne(int k)
{
  struct MyStruct* r;
  CreateMyStruct(r);
  return k + 1;
}

EMSCRIPTEN_BINDINGS(ES6Test)
{
  emscripten::function("AddOne", &AddOne);
}
