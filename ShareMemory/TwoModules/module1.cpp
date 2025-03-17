#include <emscripten.h>
#include <stdint.h>

extern "C"
{
  // Expose a function to write data into the shared memory
  EMSCRIPTEN_KEEPALIVE
  void write_data(int offset, int value)
  {
    int* ptr = (int*)offset;
    *ptr = value;
  }
}
