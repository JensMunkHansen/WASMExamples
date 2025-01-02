// Main benchmark function of MMul
// By: Nick from CoffeeBeforeArch

#include "benchmark/benchmark.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

unsigned int numThreads = std::thread::hardware_concurrency();

// Page sum
void page_sum(float* A, std::size_t N)
{
  volatile float sum = 0.0f;
  const std::size_t nFloatsPerPage = (1 << 16) >> 4;
  for (std::size_t iFloat = 0; iFloat < nFloatsPerPage; iFloat++)
  {
    for (std::size_t iPage = 0; iPage < N; iPage++)
    {
      sum += A[iPage * nFloatsPerPage + iFloat];
      A[iFloat] = sum;
    }
  }
}

// Serial MMul benchmark
static void page_bench(benchmark::State& s)
{
  // Number Dimensions of our matrix
  std::size_t N = s.range(0);

  // Create our random number generators
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_real_distribution<float> dist(-10, 10);

  // A fully page
  std::size_t nFloats = (1 << 16) >> 4 << 4; // 16 pages

  std::cout << nFloats << std::endl;
  float* A = new float[nFloats];

  // Initialize them with random values (and C to 0)
  std::generate(A, A + N, [&] { return dist(rng); });

  // Main benchmark loop
  for (auto _ : s)
  {
    page_sum(A, N);
    // std::cout << result << std::endl;
  }

  // Free memory
  delete[] A;
}
BENCHMARK(page_bench)->Arg(1)->Arg(2)->Arg(16)->Unit(benchmark::kMillisecond);

int main(int argc, char** argv)
{
  // Separate user arguments and benchmark arguments
  std::vector<char*> benchmark_args;
  for (int i = 0; i < argc; ++i)
  {
    if (std::string(argv[i]).find("--") == 0 || i == 0)
    {
      // Keep benchmark-specific arguments (starting with '--') and the program name
      benchmark_args.push_back(argv[i]);
    }
    else
    {
      // Custom user arguments
      numThreads = std::min(
        static_cast<unsigned int>(std::stoi(argv[i])), std::thread::hardware_concurrency());
    }
  }
  // Pass filtered arguments to Google Benchmark
  int benchmark_argc = static_cast<int>(benchmark_args.size());
  char** benchmark_argv = benchmark_args.data();

  benchmark::Initialize(&benchmark_argc, benchmark_argv);
  if (benchmark::ReportUnrecognizedArguments(benchmark_argc, benchmark_argv))
    return 1;
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}
