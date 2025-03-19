#include <emscripten.h>
#include <stdint.h>

static float* staticData = nullptr;

extern "C"
{
  // Expose a function to write data into the shared memory
  EMSCRIPTEN_KEEPALIVE
  void write_data(int offset, int value)
  {
    int* ptr = (int*)offset;
    *ptr = value;
  }
  EMSCRIPTEN_KEEPALIVE
  void use_heap()
  {
    staticData = (float*)malloc(3 * sizeof(float));
    staticData[0] = 0.0f;
    staticData[1] = 1.0f;
    staticData[2] = 2.0f;
  }
  EMSCRIPTEN_KEEPALIVE
  float read_heap()
  {
    return staticData[1];
  }
}
