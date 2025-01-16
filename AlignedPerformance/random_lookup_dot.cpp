#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

constexpr size_t BUFFER_SIZE = 4 * 1024 * 1024; // 4 MB buffer, interpreted as floats
constexpr size_t NUM_RANDOM_INDICES = 10000;    // Number of random indices to precompute
constexpr size_t REPETITIONS = 10000;           // Number of repetitions for performance measurement

float compute_length(float x, float y, float z)
{
  return std::sqrt(x * x + y * y + z * z);
}

int main()
{
  // Ensure buffer size is divisible by sizeof(float)
  if (BUFFER_SIZE % sizeof(float) != 0)
  {
    std::cerr << "Buffer size must be a multiple of sizeof(float)." << std::endl;
    return 1;
  }

  size_t num_floats = BUFFER_SIZE / sizeof(float);

  // Seed the random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dist(0, num_floats - 3); // Ensure we don't overflow

  // Create a buffer of floats
  std::vector<float> buffer(num_floats, 0.0f);

  // Fill the buffer with random float values
  std::uniform_real_distribution<float> float_dist(-100.0f, 100.0f);
  for (size_t i = 0; i < num_floats; ++i)
  {
    buffer[i] = float_dist(gen);
  }

  // Precompute random indices
  std::vector<size_t> random_indices(NUM_RANDOM_INDICES);
  for (size_t i = 0; i < NUM_RANDOM_INDICES; ++i)
  {
    random_indices[i] = dist(gen);
  }

  // Start performance measurement
  auto start = std::chrono::high_resolution_clock::now();

  float total_length = 0.0f;

  for (size_t repeat = 0; repeat < REPETITIONS; ++repeat)
  {
    for (size_t i = 0; i < NUM_RANDOM_INDICES; ++i)
    {
      size_t index = random_indices[i];
      float x = buffer[index];
      float y = buffer[index + 1];
      float z = buffer[index + 2];

      // Compute the length using a dot product
      total_length += compute_length(x, y, z);
    }
  }

  // End performance measurement
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  std::cout << "Completed " << REPETITIONS
            << " repetitions of random lookup and dot product operations.\n";
  std::cout << "Total time: " << elapsed.count() << " seconds.\n";
  std::cout << "Average time per operation: "
            << (elapsed.count() / (REPETITIONS * NUM_RANDOM_INDICES)) * 1e6 << " microseconds."
            << std::endl;

  // Prevent optimization of total_length
  std::cout << "Final total length (checksum): " << total_length << std::endl;

  return 0;
}
