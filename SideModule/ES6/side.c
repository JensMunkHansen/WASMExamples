#include "side.h"
#include "stdlib.h"
int side(int a)
{
  return a + 1;
}

struct MyStruct
{
  float a;
};

void CreateMyStruct(struct MyStruct* s)
{
  // I know that we leak
  struct MyStruct* newStruct = (struct MyStruct*)malloc(sizeof(struct MyStruct));
}
