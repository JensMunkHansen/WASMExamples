#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <library.h>

TEST_CASE("TestMe")
{
  std::cout << "Hello\n";
  int result = DoWork();
  REQUIRE(result == 0);
}
