#include "First.h"
#include "Types.h"
#include "stdio.h"
#include <emscripten.h>
#include <emscripten/bind.h>

int SomeStaticVariable = 1;

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

// Nasty, explicit define offset
extern "C"
{
  //__attribute__((used, section(".my_global"))) int TrueStaticVariable = 100; // Same name
  __attribute__((used, aligned(0x1000))) int TrueStaticVariable = 100;

  int EMSCRIPTEN_KEEPALIVE getTrueStaticValue()
  {
    return TrueStaticVariable;
  }
}

// This is really bad practice!!!
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
  printf("This is defined in first wasmModule\n");
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

#include <emscripten.h>

extern "C"
{
  int globalVar = 200; // Force alignment to 0x2000

  int EMSCRIPTEN_KEEPALIVE getGlobalVar()
  {
    return globalVar;
  }

  void EMSCRIPTEN_KEEPALIVE setGlobalVar(int value)
  {
    globalVar = value;
  }
}
