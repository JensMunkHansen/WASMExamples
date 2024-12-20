#include <cstdint>

#include <emscripten.h>
#include <vector>

// Expose the function to JavaScript via WebAssembly
extern "C"
{

  // Function to compute the sum of an integer array
  EMSCRIPTEN_KEEPALIVE
  int sum_array(const int* data, int length)
  {
    int sum = 0;
    for (int i = 0; i < length; i++)
    {
      sum += data[i];
    }
    return sum;
  }
}
