#include "First.h"
#include "Types.h"
#include "stdio.h"
#include <emscripten/bind.h>

// Same true for class variables!!! Dangerous
int SomeStaticVariable = 2;

extern "C"
{
  // Function to return a pointer to the start of memory
#if 0
  uint32_t* getMemoryStartSecond()
  {
    return (uint32_t*)0; // Memory starts at 0x0 in linear memory
  }
#endif
  uint32_t getMemoryStartSecond()
  {
    return (uint32_t)0x0; // Memory starts at 0x0 in linear memory
  }
}

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

extern "C"
{
  //  __attribute__((used, section(".my_global"))) int TrueStaticVariable = 200; // Same name
  __attribute__((used, aligned(0x2000))) int TrueStaticVariable = 200;
  int EMSCRIPTEN_KEEPALIVE getTrueStaticValue()
  {
    return TrueStaticVariable;
  }
}

int getGlobalStaticValue()
{
  return SomeStaticVariable;
}

// Setter function
void setGlobalStaticValue(int value)
{
  SomeStaticVariable = value;
}

int ConsumeTestCPP(struct WrappedMyData* data)
{
  printf("This is defined in second wasmModule\n");
  printf("WrappedMyData.i: %d\n", data->getInt());
  return data->getInt();
}

// Embind bindings
EMSCRIPTEN_BINDINGS(consume)
{
  // Bind ConsumeTest and allow raw pointers
  emscripten::function("ConsumeTestCPP", &ConsumeTestCPP, emscripten::allow_raw_pointers());
  emscripten::function("getGlobalStaticValue", &getGlobalStaticValue);
  emscripten::function("setGlobalStaticValue", &setGlobalStaticValue);
  emscripten::function("getTrueStaticValue", &getTrueStaticValue);
}
#endif
