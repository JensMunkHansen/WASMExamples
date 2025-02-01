#include "First.h"
#include "Types.h"
#include "stdio.h"
#include <emscripten/bind.h>

MyData Produce()
{
  MyData data;
  data.f = 1.0f;
  data.i = 2;
  return data;
}

void Consume(MyData* data)
{
  printf("f: %f, i: %d\n", data->f, data->i);
}

int ConsumeTest(MyData* data)
{
  return data->i;
}

#ifdef CPPTYPES

int ConsumeTestCPP(struct WrappedMyData* data)
{
  printf("WrappedMyData.i: %d\n", data->getInt());
  return data->getInt();
}

// Embind bindings
EMSCRIPTEN_BINDINGS(consume)
{
  // Bind ConsumeTest and allow raw pointers
  emscripten::function("ConsumeTestCPP", &ConsumeTestCPP, emscripten::allow_raw_pointers());
}
#endif
