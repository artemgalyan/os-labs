#ifndef THREAD_TESTING_TIMER_H_
#define THREAD_TESTING_TIMER_H_

#include <chrono>

class Timer {
 public:
  typedef long long time_type;
  [[nodiscard]] time_type MeasureTime();
  void Reset();
 private:
  std::chrono::steady_clock::time_point latest_ = std::chrono::steady_clock::now();
};

#endif //THREAD_TESTING_TIMER_H_
