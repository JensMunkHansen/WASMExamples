#include "array2.hpp"
#include <iostream>

using namespace arrays;

int main()
{
  ArrayWrapper<float, 3> array(5); // 5 tuples, fixed 3 components each
  array.data()[0] = 1.0f;
  array.data()[1] = 2.0f;
  array.data()[2] = 3.0f;

  std::cout << "Num Tuples: " << array.getNumTuples() << "\n";

  for (const auto& tuple : array)
  {
    for (float val : tuple)
    {
      std::cout << val << " ";
    }
    std::cout << " | ";
  }
  std::cout << "\n";

  return 0;
}
