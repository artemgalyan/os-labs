#ifndef LAB_5__WINTHREAD_H_
#define LAB_5__WINTHREAD_H_

#include <functional>
#include <stdexcept>
#include "windows.h"

class WinThread {
 private:
  template<class Function>
  struct ThreadData {
    Function function;
    void* args;
  };
 public:
  template<class Function, class ...Args>
  explicit WinThread(Function function, Args... args) {
    ThreadData<Function> data{function, &args...};
    thread_id_ = 0;
    thread_handle_ = CreateThread(nullptr, 0, [](void* data) -> DWORD {
      ThreadData<Function> i_data = *((ThreadData<Function>*)data);
      Function& f = i_data.function;
      f(*((Args*)i_data.args)...);
      return 0;
    }, &data, 0, (LPDWORD) &thread_id_);
    if (thread_handle_ == INVALID_HANDLE_VALUE) {
      throw std::runtime_error("Failed to create thread");
    }
  }
  ~WinThread() noexcept {
    if (IsJoinable())
      std::terminate();
  }
  void Join() {
    WaitForSingleObject(thread_handle_, INFINITE);
  }
  bool IsWorking() const {
    DWORD result = WaitForSingleObject(thread_handle_, 0);
      return result != WAIT_OBJECT_0;
  }
  bool IsJoinable() const {
    return IsWorking() && !detached_;
  }
  void Detach() {
    detached_ = true;
  }
  DWORD GetID() const {
    return thread_id_;
  }
 private:
  bool detached_ = false;
  DWORD thread_id_;
  HANDLE thread_handle_;
};

#endif //LAB_5__WINTHREAD_H_
