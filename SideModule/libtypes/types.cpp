#include "types.h"
#include <cstdio>
#include <cstring>

// Shared buffer for return strings
static char buffer[64];

extern "C"
{
  const char* pointToString(float x, float y, float z)
  {
    snprintf(buffer, sizeof(buffer), "(%.1f, %.1f, %.1f)", x, y, z);
    return buffer;
  }
}
