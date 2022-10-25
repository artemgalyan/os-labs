#ifndef THREAD_TESTING_MATRIXTESTER_H_
#define THREAD_TESTING_MATRIXTESTER_H_

#include "../matrix.h"

class MatrixTester {
  const int MIN_MATRIX_VALUE = -1000;
  const int MAX_MATRIX_VALUE = 1000;
  typedef long long time_type;
  struct TestResult {
    time_type duration;
    int thread_count;
  };
 public:
  explicit MatrixTester(int m, int n, int l, int block_size_);
  typedef matrix::Matrix<int> def_matrix;
  void RunTests() const;
  MatrixTester& SetStep(int new_step);
  MatrixTester& SetStartThreadsCount(int count);
  MatrixTester& CheckResults(bool value);
 private:
  bool check_results_ = true;
  void PrintResults(const std::vector<TestResult>&) const;
  def_matrix a_, b_;
  int block_size_;
  int start_threads_count_ = 1;
  int step_ = 1;
};

#endif //THREAD_TESTING_MATRIXTESTER_H_
