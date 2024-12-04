#include "../libtypes/types.h"
#include <cstdio>

int main()
{
  const char* result = pointToString(4.0f, 5.0f, 6.0f);
  printf("Module2: %s\n", result);
  return 0;
}
