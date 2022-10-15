#ifndef BUFFERED_CHANNEL_BUFFERED_CHANNEL_QUEUE_S_H_
#define BUFFERED_CHANNEL_BUFFERED_CHANNEL_QUEUE_S_H_

#include <queue>
#include <mutex>

template<class T>
class queue_s {
  std::queue<T> data_;
  mutable std::mutex mutex_;
  typedef std::lock_guard<decltype(mutex_)> lock_guard;
 public:
  bool empty() const {
    return data_.empty();
  }
  void push(const T& data) {
    lock_guard lock(mutex_);
    data_.push(data);
  }
  void push(T&& data) {
    lock_guard lock(mutex_);
    data_.push(data);
  }
  void pop() {
    lock_guard lock(mutex_);
    data_.pop();
  }
  T front() const {
    return data_.front();
  }
  size_t size() const {
    return data_.size();
  }
};

#endif //BUFFERED_CHANNEL_BUFFERED_CHANNEL_QUEUE_S_H_
