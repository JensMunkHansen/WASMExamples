#include <emscripten.h>
#include <stdint.h>
#include <stdio.h>

static float* staticData = nullptr;

extern "C"
{
  // Expose a function to read data from the shared memory
  EMSCRIPTEN_KEEPALIVE
  int read_data(int offset)
  {
    int* ptr = (int*)offset;
    return *ptr;
  }
  EMSCRIPTEN_KEEPALIVE
  void use_heap()
  {
    staticData = (float*)malloc(3 * sizeof(float));
    staticData[0] = 10.0f;
    staticData[1] = 11.0f;
    staticData[2] = 12.0f;
  }
}
