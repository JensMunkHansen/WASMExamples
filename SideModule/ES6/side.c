#include "side.h"
#include "stdio.h"
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
  // I know that we leak and we cannot even see the memory
  struct MyStruct* newStruct = (struct MyStruct*)malloc(sizeof(struct MyStruct));
  newStruct->a = 2.0f;
  printf("Side module: %3.2f\n", newStruct->a);
}

float GetValue(struct MyStruct* s)
{
  printf("Side module: GetValue() %3.2f\n", s->a);
  return s->a;
}

void SetValue(struct MyStruct* s)
{
  s->a = 2.0;
}
