#include "array.hpp"
#include <iostream>

using namespace arrays;

int main()
{
  constexpr int numComponents = 3;
  ArrayWrapper<float> array(5, numComponents); // 5 tuples, 3 components each
  array.data()[0] = 1.0f;
  array.data()[1] = 2.0f;
  array.data()[2] = 3.0f;

  std::cout << "Num Tuples: " << array.getNumTuples() << "\n";
  std::cout << "Num Components: " << array.getNumComponents() << "\n";

  auto beginIter = array.begin<numComponents>();
  auto endIter = array.end<numComponents>();

  for (auto it = beginIter; it != endIter; ++it)
  {
    for (float val : *it)
    {
      std::cout << val << " ";
    }
    std::cout << " | ";
  }

  return 0;
}
