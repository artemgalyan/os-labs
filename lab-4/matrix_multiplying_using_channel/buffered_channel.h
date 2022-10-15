#ifndef BUFFERED_CHANNEL__BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL__BUFFERED_CHANNEL_H_

#include <utility>
#include <mutex>
#include <condition_variable>

#include "queue_s.h"

template<class T>
requires std::is_default_constructible_v<T> && std::is_move_assignable_v<T> && std::is_copy_constructible_v<T>
class BufferedChannel {
  queue_s<T> queue_;
  const size_t max_size_;

  std::condition_variable read_cv_;
  std::condition_variable write_cv_;
  std::mutex write_mutex_;
  std::mutex read_mutex_;
  std::mutex working_mutex_;
  bool can_write_{};
  bool can_read_{};
  bool is_closed_ = false;
 public:
  const static bool REAL_ELEMENT = true;
  const static bool NOT_REAL_ELEMENT = false;
  explicit BufferedChannel(size_t max_size) : max_size_(max_size) {
    UpdateState();
  }

  void Send(T value) {
    std::unique_lock<std::mutex> unique_lock(write_mutex_);
    while (!can_write_) {
      if (is_closed_) {
        write_cv_.notify_all();
        throw std::runtime_error("The channel is closed.");
      }
      write_cv_.wait(unique_lock);
    }
    queue_.push(std::move(value));
    UpdateState();
    write_cv_.notify_one();
    read_cv_.notify_one();
  }

  std::pair<T, bool> Receive() {
    std::unique_lock<std::mutex> unique_lock(read_mutex_);
    while (!can_read_) {
      if (is_closed_ && queue_.empty()) {
        read_cv_.notify_all();
        return {T(), NOT_REAL_ELEMENT};
      }
      read_cv_.wait(unique_lock);
    }
    T value = queue_.front();
    queue_.pop();
    UpdateState();
    read_cv_.notify_one();
    write_cv_.notify_one();
    return {value, REAL_ELEMENT};
  }

  void Close() {
    is_closed_ = true;
    read_cv_.notify_one();
    write_cv_.notify_one();
  }
 private:
  void UpdateState() {
    can_read_ = !queue_.empty();
    can_write_ = queue_.size() <= max_size_;
  }
};

#endif //BUFFERED_CHANNEL__BUFFERED_CHANNEL_H_
