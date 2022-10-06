#ifndef THREAD_TESTING_MATRIXTESTER_H_
#define THREAD_TESTING_MATRIXTESTER_H_

#include "../matrix.h"

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
  typedef matrix::Matrix<int> def_matrix;
  void RunTests() const;
  void SetStartBlockSize(int value);
  void SetStep(int new_step);
  void SetEndBlockSize(int new_end_size);
 private:
  void PrintResults(const std::vector<TestResult>&, TestResult one_threaded) const;
  def_matrix a_, b_;
  int max_block_size_;
  int start_block_size_ = 1;
  int end_block_size_;
  int step_ = 1;
};

#endif //THREAD_TESTING_MATRIXTESTER_H_
