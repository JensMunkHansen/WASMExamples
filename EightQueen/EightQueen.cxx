#include <chrono>
#include <iostream>
#include <vector>

#ifdef __cpp_exceptions
class InvalidPlacementException : public std::exception
{
public:
  const char* what() const noexcept override { return "Invalid queen placement detected."; }
};

bool isValid(const std::vector<int>& queens, int row, int col)
{
  for (int i = 0; i < row; ++i)
  {
    if (queens[i] == col || abs(queens[i] - col) == abs(i - row))
    {
      throw InvalidPlacementException();
    }
  }
  return true;
}

void solveNQueens(
  int n, int row, std::vector<int>& queens, std::vector<std::vector<int>>& solutions)
{
  if (row == n)
  {
    solutions.push_back(queens);
    return;
  }

  for (int col = 0; col < n; ++col)
  {
    try
    {
      if (isValid(queens, row, col))
      {
        queens[row] = col;
        solveNQueens(n, row + 1, queens, solutions);
        queens[row] = -1; // Backtrack
      }
    }
    catch (const InvalidPlacementException&)
    {
      continue; // Skip invalid placements
    }
  }
}
#else
bool isValid(const std::vector<int>& queens, int row, int col)
{
  for (int i = 0; i < row; ++i)
  {
    if (queens[i] == col || abs(queens[i] - col) == abs(i - row))
    {
      return false;
    }
  }
  return true;
}

void solveNQueens(
  int n, int row, std::vector<int>& queens, std::vector<std::vector<int>>& solutions)
{
  if (row == n)
  {
    solutions.push_back(queens);
    return;
  }

  for (int col = 0; col < n; ++col)
  {
    if (isValid(queens, row, col))
    {
      queens[row] = col;
      solveNQueens(n, row + 1, queens, solutions);
      queens[row] = -1; // Backtrack
    }
  }
}
#endif

void printSolutions(const std::vector<std::vector<int>>& solutions)
{
  for (const auto& solution : solutions)
  {
    for (int row : solution)
    {
      for (int col = 0; col < solution.size(); ++col)
      {
        if (col == row)
          std::cout << "Q ";
        else
          std::cout << ". ";
      }
      std::cout << '\n';
    }
    std::cout << "\n";
  }
}

int main()
{
  constexpr int N = 8;
  std::vector<std::vector<int>> solutions;
  std::vector<int> queens(N, -1);

  auto start = std::chrono::high_resolution_clock::now();
  size_t nRepetitions = 100;
  for (size_t i = 0; i < nRepetitions; i++)
  {
    solveNQueens(N, 0, queens, solutions);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Number of solutions: " << solutions.size() << '\n';
  std::cout << "Time taken: " << duration.count() << " ms" << '\n';

  // printSolutions(solutions);

  return 0;
}
