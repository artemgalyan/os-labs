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
    joined_ = other.joined_;
    detached_ = other.detached_;
    thread_id_ = other.thread_id_;
    thread_handle_ = other.thread_handle_;
    other.joined_ = true;
    other.detached_ = false;
  }
  template<class Function, class ...Args>
  explicit WinThread(Function function, Args... args) {
    using data_pack_t = std::tuple<Args...>;
    thread_id_ = 0;
    data_pack_t args_data = std::make_tuple<Args...>(std::move(args)...);
    using passed_data_t = std::pair<Function, data_pack_t>;
    passed_data_t* data = new passed_data_t{function, args_data};
    thread_handle_ = CreateThread(nullptr, 0, &ThreadWrapperFunction<Function, Args...>, data, 0, (LPDWORD) &thread_id_);
    if (thread_handle_ == INVALID_HANDLE_VALUE) {
      delete data;
      throw std::runtime_error("Failed to create thread");
    }
  }
  ~WinThread() noexcept {
    if (IsJoinable()) {
      std::terminate();
    }
  }
  void Join() {
    if (!IsJoinable()) {
      throw std::runtime_error("Cannot join the thread");
    }
    WaitForSingleObject(thread_handle_, INFINITE);
    joined_ = true;
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
    if (joined_) {
      throw std::runtime_error("Cannot detach joined thread");
    }
    detached_ = true;
  }
  DWORD GetID() const {
    return thread_id_;
  }
 private:
  template<class Function, class ...Args>
  static DWORD ThreadWrapperFunction(void* data) {
    using data_pack_t = std::tuple<Args...>;
    using passed_data_t = std::pair<Function, data_pack_t>;
    passed_data_t* data_ptr = (passed_data_t*)data;
    passed_data_t received_data = *data_ptr;
    std::apply(received_data.first, received_data.second);
    delete data_ptr;
    return 0;
  }
  bool joined_ = false;
  bool detached_ = false;
  DWORD thread_id_;
  HANDLE thread_handle_;
};

#endif //LAB_5__WINTHREAD_H_
