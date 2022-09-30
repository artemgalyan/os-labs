#ifndef THREAD__NUMBER_MATRIX_H_
#define THREAD__NUMBER_MATRIX_H_

#include "matrix.h"

namespace matrix {

template<class T>
class NumberMatrix : public Matrix<T> {
 public:
  NumberMatrix() = default;
  NumberMatrix(int m, int n) : Matrix<T>(m, n) {};
  NumberMatrix<T> operator*(const NumberMatrix<T>& other) const {
    if (!Matrix<T>::AreMatched(*this, other)) {
      throw std::logic_error("Matrix do not match");
    }
    size_t m = this->GetM();
    size_t n = other.GetN();
    NumberMatrix<T> result(m, n);
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        result.Set(i, j, NumberMatrix<T>::Multiply(Matrix<T>::GetRow(i), other.GetColumn(j)));
      }
    }
    return result;
  }
 private:
  static T Multiply(VectorWrapper<T> a, VectorWrapper<T> b) {
    T result = 0;
    for (int i = 0; i < a.Size(); ++i) {
      result = result + a.At(i) * b.At(i);
    }
    return result;
  }
};

}
#endif //THREAD__NUMBER_MATRIX_H_
