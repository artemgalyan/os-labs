#ifndef THREAD__VECTOR_WRAPPER_H_
#define THREAD__VECTOR_WRAPPER_H_

//#define VECTOR_USE_FAKE_LOCK_GUARD // significantly improves the performance, but I recommend to use it for safety

#include <mutex>
#include <vector>

template<class T>
class VectorWrapper {
#ifdef VECTOR_USE_FAKE_LOCK_GUARD
  struct FakeLockGuard {
    explicit FakeLockGuard(std::mutex&) {}
  };
#endif
 public:
  VectorWrapper() = default;
  explicit VectorWrapper(size_t size) {
    data_ = std::vector<T>(size);
  }
  explicit VectorWrapper(size_t size, T element) {
    data_ = std::vector<T>(size, element);
  }
  explicit VectorWrapper(std::vector<T>&& v) : data_(std::move(v)) {}
  explicit VectorWrapper(std::vector<T> v) : data_(std::move(v)) {}
  VectorWrapper<T>& operator=(const VectorWrapper<T>& a) {
    if (this == &a)
      return *this;
    lock_guard lock(lock_);
    data_.clear();
    for (auto i = 0; i < a.Size(); ++i) {
      data_.push_back(a.At(i));
    }
    return *this;
  };
  VectorWrapper(const VectorWrapper& a) {
    lock_guard lock(lock_);
    data_.resize(a.data_.size());
    std::copy(a.data_.begin(), a.data_.end(), data_.begin());
  };
  void PushBack(T value) {
    lock_guard lock(lock_);
    data_.push_back(value);
  }

  T& At(size_t index) {
    return data_[index];
  }

  const T& At(size_t index) const {
    return data_[index];
  }

  const T* AtPtr(size_t index) const {
    return &data_[index];
  }

  void Set(size_t index, T value) {
    lock_guard lock(lock_);
    data_[index] = value;
  }

  bool IsEmpty() const {
    return data_.empty();
  }

  size_t Size() const {
    return data_.size();
  }

  template<class ...Args>
  void EmplaceBack(Args ...args) {
    lock_guard lock(lock_);
    data_.emplace_back(args...);
  }
 private:
  mutable std::mutex lock_;
  std::vector<T> data_;
#ifdef VECTOR_USE_FAKE_LOCK_GUARD
  typedef FakeLockGuard lock_guard;
#else
  typedef std::lock_guard<decltype(lock_)> lock_guard;
#endif
};

#endif //THREAD__VECTOR_WRAPPER_H_
