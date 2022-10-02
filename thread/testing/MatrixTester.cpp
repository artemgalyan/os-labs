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
  std::cout << "Starting tests.." << std::endl;
  std::vector<TestResult> results;
  Timer timer;
  for (int block_size = 1; block_size < max_block_size_; ++block_size) {
    std::cout << "Running test for block size " << block_size << std::endl;
    TestResult tr;
    tr.block_size = block_size;
    timer.Reset();
    matrix::MultiplyMultithreaded(a_, b_, block_size);
    tr.duration = timer.MeasureTime();
    results.push_back(tr);
  }
  std::cout << "Running test for 1 thread" << std::endl;
  timer.Reset();
  auto res = a_ * b_;
  auto duration = timer.MeasureTime();
  PrintResults(results, {duration, -1});
}

struct separate_thousands : std::numpunct<char> {
  char_type do_thousands_sep() const override { return ','; }
  string_type do_grouping() const override { return "\3"; }
};

void MatrixTester::PrintResults(const std::vector<TestResult>& res, TestResult one_threaded) const {
  std::cout << "Test for m = " << a_.GetM() << ", n = " << a_.GetN() << ", l = " << b_.GetN() << std::endl;
  auto fastest = *std::min_element(res.begin(), res.end(), [](const TestResult& a, const TestResult& b) {
    return a.duration < b.duration;
  });
  auto thousands = std::make_unique<separate_thousands>();
  std::cout.imbue(std::locale(std::cout.getloc(), thousands.release()));
  std::cout << std::setw(15) << "description" << " | " << std::setw(15) << "time (microseconds)" << " | "
            << "% to 1 threaded"
            << std::endl;
  std::cout << std::setw(15) << "1 threaded" << " | " << std::setw(15) << one_threaded.duration << " | " << "100%"
            << std::endl;
  for (const auto& r : res) {
    std::string info_string = std::to_string(r.block_size) + " block size";
    std::cout << std::setw(15) << info_string << " | " << std::setw(15) << r.duration << " | "
              << (double) r.duration / one_threaded.duration * 100 << '%';
    if (r.duration == fastest.duration) {
      std::cout << "\t<-- fastest";
    }
    std::cout << std::endl;
  }
  std::cout << "Fastest is: block size is " << fastest.block_size << ", time is " << fastest.duration << " ("
            << (double) fastest.duration / one_threaded.duration * 100 << "%) from 1 threaded." << std::endl;
}
