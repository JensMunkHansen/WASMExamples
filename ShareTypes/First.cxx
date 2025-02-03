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

/*
  To avoid sharing static content, one can write a linker script module1.ld and
  append this linker argument '-Wl,-T,module1.ld'. The script contains

SECTIONS {
    .mydata1 0x2000 : {
        KEEP(*(.my_global)) //
    }
}

and the static variable should be defined with an attribute

__attribute__((used, section(".mydata1")))
int globalVar = 100;  // Placed in .mydata1

*/

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
}

#endif
