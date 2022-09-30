#ifndef THREAD__VECTOR_WRAPPER_H_
#define THREAD__VECTOR_WRAPPER_H_

#include <mutex>
#include <vector>

template<class T>
class VectorWrapper {
  typedef unsigned long long size_type;
 public:
  VectorWrapper() = default;
  explicit VectorWrapper(size_type size) {
    data_ = std::vector<T>(size);
  }
  explicit VectorWrapper(size_type size, T element) {
    data_ = std::vector<T>(size, element);
  }
  VectorWrapper<T>& operator=(const VectorWrapper<T>& a) {
    if (this == &a)
      return *this;
    lock_.lock();
    data_.clear();
    for (auto i = 0; i < a.Size(); ++i) {
      data_.push_back(a.At(i));
    }
    lock_.unlock();
    return *this;
  };
  VectorWrapper(const VectorWrapper& a) {
    lock_.lock();
    data_.clear();
    for (auto i = 0; i < a.Size(); ++i) {
      data_.push_back(a.At(i));
    }
    lock_.unlock();
  };
  void PushBack(T value) {
    lock_.lock();
    data_.push_back(value);
    lock_.unlock();
  }

  T At(size_type index) const {
    return data_.at(index);
  }

  T& AtRef(size_type index) {
    lock_.lock();
    T& value = data_[index];
    lock_.unlock();
    return value;
  }

  void Set(size_type index, T value) {
    lock_.lock();
    data_[index] = value;
    lock_.unlock();
  }

  [[nodiscard]] bool IsEmpty() const {
    return data_.empty();
  }

  [[nodiscard]] size_type Size() const {
    return data_.size();
  }
 private:
  std::mutex lock_;
  std::vector<T> data_;
};

#endif //THREAD__VECTOR_WRAPPER_H_
