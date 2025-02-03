#ifndef first_h
#define first_h

#include "Types.h"

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
#endif

extern int SomeStaticVariable;

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

#endif
