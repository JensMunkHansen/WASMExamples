#include <chrono>
#include <iostream>
#include <random>
#include <vector>

constexpr size_t BUFFER_SIZE = 4 * 1024 * 1024; // 4 MB buffer
constexpr size_t NUM_RANDOM_ADDRESSES = 10000;  // Number of random addresses to precompute
constexpr size_t REPETITIONS = 100;             // Number of repetitions for performance measurement

int main()
{
  // Seed the random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dist(0, BUFFER_SIZE - 1);

  // Create a buffer of 4 MB
  std::vector<uint8_t> buffer(BUFFER_SIZE, 0);

  // Precompute random addresses
  std::vector<size_t> random_addresses(NUM_RANDOM_ADDRESSES);
  for (size_t i = 0; i < NUM_RANDOM_ADDRESSES; ++i)
  {
    random_addresses[i] = dist(gen);
  }

  // Start performance measurement
  auto start = std::chrono::high_resolution_clock::now();

  for (size_t repeat = 0; repeat < REPETITIONS; ++repeat)
  {
    for (size_t i = 0; i < NUM_RANDOM_ADDRESSES; ++i)
    {
      size_t random_index = random_addresses[i];

      // Write a random value
      uint8_t random_value = static_cast<uint8_t>(dist(gen) % 256);
      buffer[random_index] = random_value;

      // Read back the value
      volatile uint8_t read_value = buffer[random_index]; // Volatile to prevent optimization
    }
  }

  // End performance measurement
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  std::cout << "Completed " << REPETITIONS << " repetitions of random read/write operations.\n";
  std::cout << "Total time: " << elapsed.count() << " seconds.\n";
  std::cout << "Average time per operation: "
            << (elapsed.count() / (REPETITIONS * NUM_RANDOM_ADDRESSES)) * 1e6 << " microseconds."
            << std::endl;

  return 0;
}
