#include "matrix_utils.h"

namespace matrix {
NumberMatrix<int> GenerateMatrix(int m, int n, int min_value, int max_value) {
  NumberMatrix<int> result(m, n);
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(min_value, max_value);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      result.Set(i, j, dist(mt));
    }
  }
  return result;
}
}