#include <emscripten.h>
#include <stdint.h>

extern "C"
{
  // Expose a function to read data from the shared memory
  EMSCRIPTEN_KEEPALIVE
  int read_data(int offset)
  {
    int* ptr = (int*)offset;
    return *ptr;
  }
}
