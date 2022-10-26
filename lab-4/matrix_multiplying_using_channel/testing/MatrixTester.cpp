#include "MatrixTester.h"
#include "../matrix_utils.h"
#include "Timer.h"

#include <iomanip>

MatrixTester::MatrixTester(int m, int n, int l, int block_size) : block_size_(block_size) {
  a_ = matrix::GenerateMatrix(m, n, MIN_MATRIX_VALUE, MAX_MATRIX_VALUE);
  b_ = matrix::GenerateMatrix(n, l, MIN_MATRIX_VALUE, MAX_MATRIX_VALUE);
}

void MatrixTester::RunTests() const {
  std::cout << "Starting tests.." << std::endl;
  std::vector<TestResult> results;
  Timer timer;
  std::cout << "Initializing..." << std::endl;
  def_matrix result;
  if (check_results_)
    result = a_ * b_;
  int max_number_of_threads = ceil((double) a_.GetM() / block_size_) * ceil((double) a_.GetN() / block_size_)
      * ceil((double) b_.GetN() / block_size_) * ceil((double) b_.GetM() / block_size_);
  for (int threads = start_threads_count_; threads <= max_number_of_threads; threads += step_) {
    std::cout << "Running test for " << threads << " thread(s). " << std::endl;
    TestResult tr;
    tr.thread_count = threads;
    timer.Reset();
    auto c = matrix::MultiplyMultithreaded(a_, b_, block_size_, threads);
    tr.duration = timer.MeasureTime();
    if (check_results_ && c != result) {
      std::cerr << "An error occurred while multiplying matrices with " << threads << " threads" << std::endl;
    }
    results.push_back(tr);
  }
  PrintResults(results);
}

struct separate_thousands : std::numpunct<char> {
  char_type do_thousands_sep() const override { return ','; }
  string_type do_grouping() const override { return "\3"; }
};

void MatrixTester::PrintResults(const std::vector<TestResult>& res) const {
  std::cout << "Test for m = " << a_.GetM() << ", n = " << a_.GetN() << ", l = " << b_.GetN() << std::endl;
  auto fastest = *std::min_element(res.begin(), res.end(), [](const TestResult& a, const TestResult& b) {
    return a.duration < b.duration;
  });
  auto thousands = std::make_unique<separate_thousands>();
  std::cout.imbue(std::locale(std::cout.getloc(), thousands.release()));
  std::cout << std::setw(15) << "description" << " | " << std::setw(15) << "time (microseconds)" << " | "
            << "% to 1 threaded"
            << std::endl;
  for (const auto& r : res) {
    std::string info_string = std::to_string(r.thread_count) + " threads";
    std::cout << std::setw(15) << info_string << " | " << std::setw(15) << r.duration;
    if (r.duration == fastest.duration) {
      std::cout << "\t<-- fastest";
    }
    std::cout << std::endl;
  }
  std::cout << "Fastest is: " << fastest.thread_count << " threads, time is " << fastest.duration << std::endl;
}

MatrixTester& MatrixTester::CheckResults(bool value) {
  check_results_ = value;
  return *this;
}
MatrixTester& MatrixTester::SetStep(int new_step) {
  step_ = new_step;
  return *this;
}
MatrixTester& MatrixTester::SetStartThreadsCount(int count) {
  start_threads_count_ = count;
  return *this;
}
