
#include "MatrixTester.h"
#include "../matrix_utils.h"
#include "Timer.h"

#include <iomanip>

MatrixTester::MatrixTester(int m, int n, int l) {
  a_ = matrix::GenerateMatrix(m, n, MIN_MATRIX_VALUE, MAX_MATRIX_VALUE);
  b_ = matrix::GenerateMatrix(n, l, MIN_MATRIX_VALUE, MAX_MATRIX_VALUE);
  max_block_size_ = std::min(m, std::min(n, l)) + 1;
}

void MatrixTester::RunTests() const {
  std::vector<TestResult> results;
  Timer timer;
  for (int block_size = 1; block_size < max_block_size_; ++block_size) {
    TestResult tr;
    tr.block_size = block_size;
    timer.Reset();
    matrix::MultiplyMultithreaded(a_, b_, block_size);
    tr.duration = timer.MeasureTime();
    results.push_back(tr);
  }
  timer.Reset();
  auto res = a_ * b_;
  auto duration = timer.MeasureTime();
  PrintResults(results, {duration, -1});
}

void MatrixTester::PrintResults(const std::vector<TestResult>& res, TestResult one_threaded) const {
  std::cout << std::setw(15) << "description" << " | " << std::setw(15) << "time (microseconds)" << " | "
            << "% to 1 threaded"
            << std::endl;
  std::cout << std::setw(15) << "1 threaded" << " | " << std::setw(15) << one_threaded.duration << " | " << "100%"
            << std::endl;
  for (const auto& r : res) {
    std::string info_string = std::to_string(r.block_size) + " block size";
    std::cout << std::setw(15) << info_string << " | " << std::setw(15) << r.duration << " | "
              << (double) r.duration / one_threaded.duration * 100 << '%' << std::endl;
  }
}
