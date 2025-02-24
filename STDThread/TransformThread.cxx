#include <algorithm>
#include <execution> // For parallel execution policies
#include <iostream>
#include <vector>

int main()
{
  std::vector<int> data(1000000, 1); // 1 million elements initialized to 1
  std::vector<int> result(data.size());

  // Use parallel execution to transform elements
  std::transform(
    std::execution::par, data.begin(), data.end(), result.begin(), [](int x) { return x * 2; });

  // Print some results
  std::cout << "First 10 results: ";
  for (int i = 0; i < 10; ++i)
  {
    std::cout << result[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
