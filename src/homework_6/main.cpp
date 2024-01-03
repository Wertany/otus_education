#include <cassert>

#include "fmt/core.h"
#include "homework_6.hpp"

int main() {
  homework_6::Matrix<int, -1> matrix;
  assert(matrix.size() == 0);

  auto a = matrix[0][0];
  assert(a == -1);
  assert(matrix.size() == 0);

  matrix[100][100] = 314;
  assert(matrix[100][100] == 314);
  assert(matrix.size() == 1);

  for (auto c : matrix) {
    auto [key, value] = c;
    auto [x, y] = key;
    fmt::print("[{}] [{}] value {}\n", x, y, value);
  }

  ((matrix[100][100] = 314) = 0) = 217;
  fmt::print("matrix[100][100] {}\n", matrix[100][100].GetValue());
}
