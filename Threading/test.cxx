#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <library.h>
#include <thread>

TEST_CASE("TestMe")
{
  std::cout << "Hello\n";
  // Makes it stuck
  // size_t nMaxThreads = static_cast<std::size_t>(std::thread::hardware_concurrency());

  //  std::cout << nMaxThreads << std::endl;

  std::cout << static_cast<std::size_t>(EM_ASM_INT({ return PThread.unusedWorkers.length }))
            << std::endl;
  std::cout << "Test.cxx" << std::endl;
  int result = DoWork();

  REQUIRE(result == 0);
}
