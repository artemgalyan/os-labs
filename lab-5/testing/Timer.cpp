#include "Timer.h"

Timer::time_type Timer::MeasureTime() {
  auto now = std::chrono::steady_clock::now();
  auto diff = now - latest_;
  latest_ = now;
  return std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
}
void Timer::Reset() {
  latest_ = std::chrono::steady_clock::now();
}
