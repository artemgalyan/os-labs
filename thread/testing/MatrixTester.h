#ifndef THREAD_TESTING_MATRIXTESTER_H_
#define THREAD_TESTING_MATRIXTESTER_H_

#include "../matrix.h"
#include "../number_matrix.h"

class MatrixTester {
  const int MIN_MATRIX_VALUE = -1000;
  const int MAX_MATRIX_VALUE = 1000;
  typedef long long time_type;
  struct TestResult {
    time_type duration;
    int block_size;
  };
 public:
  explicit MatrixTester(int m, int n, int l);
  typedef matrix::NumberMatrix<int> def_matrix;
  void RunTests() const;
 private:
  void PrintResults(const std::vector<TestResult>&, TestResult one_threaded) const;
  def_matrix a_, b_;
  int max_block_size_;
};

#endif //THREAD_TESTING_MATRIXTESTER_H_