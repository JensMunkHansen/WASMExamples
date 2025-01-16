#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

// Configuration
constexpr size_t BUFFER_ALIGNMENT = 16;         // 32-byte alignment for SIMD
constexpr size_t BUFFER_SIZE = 4 * 1024 * 1024; // Total buffer size in bytes
constexpr size_t ELEMENT_SIZE = sizeof(std::array<float, 4>);
constexpr size_t NUM_ELEMENTS = BUFFER_SIZE / ELEMENT_SIZE; // Number of elements
constexpr size_t NUM_RANDOM_INDICES = 10000;                // Number of random lookups
constexpr size_t REPETITIONS = 10000; // Repetitions for performance measurement

// Custom aligned allocator
template <typename T, std::size_t Alignment>
struct AlignedAllocator
{
  using value_type = T;

  AlignedAllocator() noexcept = default;

  template <typename U>
  AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept
  {
  }

  T* allocate(std::size_t n)
  {
    void* ptr = nullptr;
    if (posix_memalign(&ptr, Alignment, n * sizeof(T)) != 0)
    {
      throw std::bad_alloc();
    }
    return static_cast<T*>(ptr);
  }

  void deallocate(T* p, std::size_t) noexcept { free(p); }

  template <typename U>
  struct rebind
  {
    using other = AlignedAllocator<U, Alignment>;
  };
};

template <typename T, std::size_t Alignment>
bool operator==(const AlignedAllocator<T, Alignment>&, const AlignedAllocator<T, Alignment>&)
{
  return true;
}

template <typename T, std::size_t Alignment>
bool operator!=(const AlignedAllocator<T, Alignment>&, const AlignedAllocator<T, Alignment>&)
{
  return false;
}

// Compute the length of a 3D vector stored in std::array<float, 4>
float compute_length(const std::array<float, 4>& vec)
{
  return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

int main()
{
  // Define a vector of aligned std::array<float, 4>
  using AlignedVector =
    std::vector<std::array<float, 4>, AlignedAllocator<std::array<float, 4>, BUFFER_ALIGNMENT>>;
  AlignedVector buffer(NUM_ELEMENTS);

  // Seed the random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dist(0, NUM_ELEMENTS - 1);
  std::uniform_real_distribution<float> float_dist(-100.0f, 100.0f);

  // Fill the buffer with random values
  for (size_t i = 0; i < NUM_ELEMENTS; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      buffer[i][j] = float_dist(gen);
    }
  }

  // Verify alignment
  if (reinterpret_cast<std::uintptr_t>(buffer.data()) % BUFFER_ALIGNMENT == 0)
  {
    std::cout << "Buffer is aligned to " << BUFFER_ALIGNMENT << " bytes." << std::endl;
  }
  else
  {
    std::cerr << "Buffer alignment failed!" << std::endl;
    return 1;
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
      const auto& vec = buffer[index];
      total_length += compute_length(vec);
    }
  }

  // End performance measurement
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  // Output results
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
