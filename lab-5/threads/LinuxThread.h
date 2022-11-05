#ifndef LAB_5__LINUXTHREAD_H_
#define LAB_5__LINUXTHREAD_H_

#include <pthread.h>
#include <functional>

class LinuxThread {
 public:
  const static size_t MAX_NUMBER_OF_THREADS = 20000;
  template<class Function, class ...Args>
  explicit LinuxThread(Function function, Args... args) {
    using data_pack_t = std::tuple<Args...>;
    data_pack_t pack = std::make_tuple<Args...>(std::move(args)...);
    using passed_data_t = std::pair<Function, data_pack_t>;
    data_ = new passed_data_t(function, pack);
    void* (*thread_func)(void*) = [](void* data) -> void* {
      passed_data_t recv_data = *((passed_data_t*)data);
      std::apply(recv_data.first, recv_data.second);
      return nullptr;
    };
    int result = pthread_create(&thread_id_, nullptr, thread_func, data_);
    if (result != 0) {
      delete (int*)data_;
      data_ = nullptr;
      throw std::runtime_error("Failed to create thread, return code " + std::to_string(result));
    }
  }
  LinuxThread() {
    joined_ = true;
    detached_ = false;
    thread_id_ = 0;
    data_ = nullptr;
  }
  LinuxThread(LinuxThread&& other) noexcept {
    data_ = other.data_;
    other.data_ = nullptr;
    joined_ = other.joined_;
    detached_ = other.detached_;
    thread_id_ = other.thread_id_;
    other.joined_ = true;
    other.detached_ = false;
  }
  ~LinuxThread() {
    if (IsJoinable()) {
      std::terminate();
    }
  }
  void Join() {
    if (!IsJoinable()) {
      throw std::runtime_error("Cannot join the thread");
    }
    pthread_join(thread_id_, nullptr);
    delete (int*)data_;
    data_ = nullptr;
    joined_ = true;
  }
  bool IsJoinable() const {
    if (detached_) {
      return false;
    }
    return !joined_;
  }
  pthread_t GetID() const {
    return thread_id_;
  }
  void Detach() {
    if (joined_) {
      throw std::runtime_error("Cannot detach joined thread");
    }
    detached_ = true;
  }
 private:
  pthread_t thread_id_;
  void* data_;
  bool joined_ = false;
  bool detached_ = false;
};

#endif //LAB_5__LINUXTHREAD_H_
