#include "../libtypes/types.h"
#include "stdio.h"

int main()
{
  const char* result = pointToString(1.0f, 2.0f, 3.0f);
  printf("Module1: %s\n", result);
  return 0;
}
