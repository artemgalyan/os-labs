#ifndef BUFFERED_CHANNEL__BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL__BUFFERED_CHANNEL_H_

#include <utility>
#include <mutex>
#include <queue>
#include <condition_variable>

template<class T> requires std::is_default_constructible_v<T> && std::is_move_assignable_v<T>
    && std::is_copy_constructible_v<T>
class BufferedChannel {
  std::queue<T> queue_;
  const size_t max_size_;

  std::condition_variable condition_variable_;
  std::mutex mutex_;
  bool is_closed_ = false;
 public:
  constexpr const static bool REAL_ELEMENT = true;
  constexpr const static bool NOT_REAL_ELEMENT = false;
  explicit BufferedChannel(size_t max_size) : max_size_(max_size) {}

  void Send(T value) {
    if (is_closed_) {
      throw std::runtime_error("The channel is closed.");
    }
    std::unique_lock<std::mutex> unique_lock(mutex_);
    condition_variable_.wait(unique_lock, [&]() {
      return queue_.size() < max_size_ || is_closed_;
    });
    if (is_closed_) {
      throw std::runtime_error("The channel is closed.");
    }
    if (is_closed_) {
      condition_variable_.notify_one();
      throw std::runtime_error("The channel is closed.");
    }
    queue_.push(std::move(value));
    condition_variable_.notify_one();
  }

  std::pair<T, bool> Receive() {
    std::unique_lock<std::mutex> unique_lock(mutex_);
    condition_variable_.wait(unique_lock, [&]() {
      return !queue_.empty() || is_closed_;
    });
    if (queue_.empty() && is_closed_) {
      condition_variable_.notify_one();
      return {T(), NOT_REAL_ELEMENT};
    }
    T value = queue_.front();
    queue_.pop();
    condition_variable_.notify_one();
    return {value, REAL_ELEMENT};
  }

  void Close() {
    std::lock_guard<std::mutex> lock(mutex_);
    is_closed_ = true;
    condition_variable_.notify_one();
  }
};

#endif //BUFFERED_CHANNEL__BUFFERED_CHANNEL_H_
