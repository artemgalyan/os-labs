#ifndef MY_THREAD__MY_MATRIX_H_
#define MY_THREAD__MY_MATRIX_H_

#include <cmath>

#include "vector_wrapper.h"

namespace matrix {

template<typename T>
concept MatrixType = requires(T a, T b) {
  { a + b } -> std::same_as<T>;
  { a * b } -> std::same_as<T>;
  std::copy_constructible<T>;
  std::assignable_from<T, T>;
};

template<MatrixType T>
class Matrix {
 public:
  explicit Matrix(const std::vector<std::vector<T>>& vector) {
    for (const auto& line : vector) {
      data_.PushBack(VectorWrapper<T>(line));
    }
    m_ = data_.Size();
    n_ = data_.At(0).Size();
  }
  explicit Matrix(int m, int n) : m_(m), n_(n) {
    data_ = VectorWrapper<VectorWrapper<T>>(m, VectorWrapper<T>(n));
  }
  Matrix() : Matrix(0, 0) {};
  Matrix(const Matrix<T>&) = default;

  static bool AreMatched(const Matrix<T>& a, const Matrix<T>& b) {
    return a.n_ == b.m_;
  }
  T Get(size_t i, size_t j) const {
    return data_.At(i).At(j);
  }
  T& GetRef(size_t i, size_t j) {
    return data_.At(i).At(j);
  }
  void Set(size_t i, size_t j, T value) {
    data_.At(i).Set(j, value);
  }
  [[nodiscard]] int GetM() const {
    return m_;
  }
  [[nodiscard]] int GetN() const {
    return n_;
  }
  VectorWrapper<T> GetRow(int row) const {
    return data_.At(row);
  }
  VectorWrapper<T> GetColumn(int column) const {
    VectorWrapper<T> result;
    for (int i = 0; i < m_; ++i) {
      result.PushBack(data_.At(i).At(column));
    }
    return result;
  }
  VectorWrapper<const T*> GetRowPtr(int row) const {
    VectorWrapper<const T*> result(n_);
    for (int i = 0; i < n_; ++i) {
      result.Set(i, data_.AtPtr(row)->AtPtr(i));
    }
    return result;
  }
  VectorWrapper<const T*> GetColumnPtr(int column) const {
    VectorWrapper<const T*> result(m_);
    for (int i = 0; i < m_; ++i) {
      result.Set(i, data_.AtPtr(i)->AtPtr(column));
    }
    return result;
  }
  Matrix operator+(const Matrix<T>& other) const {
    if (other.n_ != n_ || other.m_ != m_)
      throw std::logic_error("Matrices have different sizes!");
    Matrix<T> matrix(*this);
    int m = m_;
    int n = n_;
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        matrix.Set(i, j, matrix.Get(i, j) + other.Get(i, j));
      }
    }
    return matrix;
  }
  Matrix<T>& operator+=(const Matrix<T>& other) {
    if (other.n_ != n_ || other.m_ != m_)
      throw std::logic_error("Matrices have different sizes!");
    static std::mutex mutex;
    mutex.lock();
    int m = m_;
    int n = n_;
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        Set(i, j, Get(i, j) + other.Get(i, j));
      }
    }
    mutex.unlock();
    return *this;
  }
  Matrix<T> operator*(const Matrix<T>& other) const {
    if (!AreMatched(*this, other)) {
      throw std::logic_error("Matrices do not match");
    }
    size_t m = this->GetM();
    size_t n = other.GetN();
    Matrix<T> result(m, n);
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        result.Set(i, j, Multiply(Matrix<T>::GetRow(i), other.GetColumn(j)));
      }
    }
    return result;
  }
  Matrix<T>& operator=(const Matrix<T>& other) {
    if (&other == this) {
      return *this;
    }
    m_ = other.m_;
    n_ = other.n_;
    data_ = other.data_;
    return *this;
  }
  bool operator==(const Matrix<T>& other) const {
    if (m_ != other.m_ || n_ != other.n_)
      return false;
    if (this == &other)
      return true;
    for (auto i = 0; i < m_; ++i) {
      for (auto j = 0; j < n_; ++j) {
        if (Get(i, j) != other.Get(i, j))
          return false;
      }
    }
    return true;
  }
  bool operator!=(const Matrix<T>& other) const {
    return !(*this == other);
  }
 private:
  static T Multiply(const VectorWrapper<T>& a, const VectorWrapper<T>& b) {
    if (a.IsEmpty() && b.IsEmpty()) {
      throw std::invalid_argument("Vectors are empty!");
    }
    if (a.Size() != b.Size()) {
      throw std::invalid_argument("Vectors have different size!");
    }
    T result = a.At(0) * b.At(0);
    for (int i = 1; i < a.Size(); ++i) {
      result = result + a.At(i) * b.At(i);
    }
    return result;
  }
  int m_{};
  int n_{};
  VectorWrapper<VectorWrapper<T>> data_;
};
}
#endif
