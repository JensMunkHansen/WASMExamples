#include "array2.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

using namespace arrays;

int main()
{
  ArrayWrapper<float, 3> array(5); // 5 tuples, fixed 3 components each
  for (size_t i = 0; i < 15; i++)
  {
    array.data()[i] = (float)i;
  }

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

  // Compute the lengths of each tuple using std::transform
  std::vector<float> lengths(array.getNumTuples());
  std::transform(array.begin(), array.end(), lengths.begin(),
    [](const std::array<float, 3>& tuple)
    { return std::sqrt(tuple[0] * tuple[0] + tuple[1] * tuple[1] + tuple[2] * tuple[2]); });

  std::cout << "Tuple lengths: ";
  for (float len : lengths)
  {
    std::cout << len << " ";
  }

  return 0;
}
