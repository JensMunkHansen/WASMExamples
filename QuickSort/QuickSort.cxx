#include <algorithm> // For std::sort
#include <chrono>
#include <iostream>
#include <random> // For random number generation
#include <vector>

int main()
{
  // Number of elements
  constexpr size_t N = 1000;

  // Random number generator setup
  std::random_device rd;                                    // Seed
  std::mt19937 gen(rd());                                   // Mersenne Twister PRNG
  std::uniform_real_distribution<float> dist(0.0f, 100.0f); // Float range [0, 100]

  // Generate a random vector of floats
  std::vector<float> numbers(N);
  for (float& num : numbers)
  {
    num = dist(gen);
  }

  auto start = std::chrono::high_resolution_clock::now();
  size_t nRepetitions = 1000;

  // Out-of-place sorting: Create a sorted copy
  std::vector<float> sorted_numbers = numbers; // Copy original vector
  float result = 0.0;
  for (size_t i = 0; i < nRepetitions; i++)
  {
    std::sort(sorted_numbers.begin(), sorted_numbers.end());
    result += sorted_numbers[0];
    sorted_numbers = numbers;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Time taken: " << duration.count() << " ms" << '\n';
  std::cout << "Result: " << result << '\n';
  // printSolutions(solutions);

  return 0;
}
