#ifndef first_h
#define first_h

#include "Types.h"
#include <emscripten.h>

#ifdef __cplusplus
extern "C"
{
#endif

  MyData Produce();
  void Consume(MyData* data);
  int ConsumeTest(MyData* data);
#ifdef __cplusplus
}
#endif

#ifdef CPPTYPES
int ConsumeTestCPP(struct WrappedMyData* data);

// Dangerous
extern int SomeStaticVariable;

// This is really bad practice!!!
int getGlobalStaticValue();

// Setter function
void setGlobalStaticValue(int value);
#endif

#endif
