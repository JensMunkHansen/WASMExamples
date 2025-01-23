#include <iostream>
#include <limits>

int main()
{
  int max = std::numeric_limits<int>::max();
  int result = max + 1; // Signed integer overflow: Undefined Behavior

  std::cout << "Result: " << result << std::endl; // UBSan should catch this
  return 0;
}
