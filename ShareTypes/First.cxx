#include "First.h"
#include "Types.h"
#include "stdio.h"

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
