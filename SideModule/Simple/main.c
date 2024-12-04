#include "side.h"
#include <stdio.h>

int side(int a);
int main()
{
  struct MyStruct* r;
  CreateMyStruct(r);
  printf("hello world %d\n", side(1));
}
