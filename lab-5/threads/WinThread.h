#ifndef LAB_5__WINTHREAD_H_
#define LAB_5__WINTHREAD_H_

#include <functional>
#include <stdexcept>
#include "windows.h"

class WinThread {
 public:
  WinThread() {
    joined_ = true;
    detached_ = false;
    thread_id_ = 0;
    thread_handle_ = nullptr;
  }
  WinThread(const WinThread&) = delete;
  WinThread(WinThread&& other)  noexcept {
    data_ = other.data_;
    other.data_ = nullptr;
    joined_ = other.joined_;
    detached_ = other.detached_;
    thread_id_ = other.thread_id_;
    thread_handle_ = other.thread_handle_;
    other.joined_ = true;
    other.detached_ = false;
  }
  template<class Function, class ...Args>
  explicit WinThread(Function function, Args... args) {
    using data_t = std::tuple<Args...>;
    thread_id_ = 0;
    data_t args_data = std::make_tuple<Args...>(std::move(args)...);
    data_ = new std::pair<Function, data_t>{function, args_data};
    thread_handle_ = CreateThread(nullptr, 0, [](void* data) -> DWORD {
      std::pair<Function, data_t> d = *((std::pair<Function, data_t>*) data);
      std::apply(d.first, d.second);
      return 0;
    }, data_, 0, (LPDWORD) &thread_id_);
    if (thread_handle_ == INVALID_HANDLE_VALUE) {
      throw std::runtime_error("Failed to create thread");
    }
  }
  ~WinThread() noexcept {
    if (IsJoinable()) {
      std::terminate();
    }
  }
  void Join() {
    WaitForSingleObject(thread_handle_, INFINITE);
    joined_ = true;
    delete (int*)data_;
    data_ = nullptr;
  }
  bool IsWorking() const {
    DWORD result = WaitForSingleObject(thread_handle_, 0);
    return result != WAIT_OBJECT_0;
  }
  bool IsJoinable() const {
    if (detached_) {
      return false;
    }
    return !joined_;
  }
  void Detach() {
    detached_ = true;
  }
  DWORD GetID() const {
    return thread_id_;
  }
 private:
  void* data_ = nullptr;
  bool joined_ = false;
  bool detached_ = false;
  DWORD thread_id_;
  HANDLE thread_handle_;
};

#endif //LAB_5__WINTHREAD_H_
