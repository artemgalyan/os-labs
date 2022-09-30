#ifndef THREAD__MATRIX_OLD_H_
#define THREAD__MATRIX_OLD_H_

#include <array>
#include <vector>
#include <thread>

namespace math {

template<int _m, int _n, class value_type>
class Matrix_old {
 public:
  template<int l>
  Matrix_old<_m, l, value_type> operator*(const Matrix_old<_n, l, value_type>& matrix) {
    Matrix_old<_m, l, value_type> result;
    for (int i = 0; i < _m; ++i) {
      for (int j = 0; j < l; ++j) {
        value_type element = Multiply(GetRow(i), matrix.GetColumn(j));
        result.Set(i, j, element);
      }
    }
    return result;
  }
  void Set(int i, int j, value_type value) {
    data_[i][j] = value;
  }
  [[nodiscard]] value_type Get(int i, int j) const {
    return data_[i][j];
  }
  Matrix_old<_n, _m, value_type> T() const {
    Matrix_old<_n, _m, value_type> result;
    for (int i = 0; i < _m; ++i)
      Copy(GetRow(i), result.data_[i]);
    return result;
  }
 private:
  template<int k, class T>
  void Copy(const std::array<T, k>& from, std::array<T, k>& to) {
    for (int i = 0; i < k; ++i)
      to[i] = from[i];
  }
  std::array<value_type, _n> GetRow(int m) const {
    return data_[m];
  }
  std::array<value_type, _m> GetColumn(int n) const {
    std::array<value_type, _m> result;
    for (int i = 0; i < _m; ++i) {
      result[i] = data_[i][n];
    }
    return result;
  }
  template<int l>
  static value_type Multiply(const std::array<value_type, l>& a, const std::array<value_type, l>& b) {
    value_type result = 0;
    for (int i = 0; i < l; ++i) {
      result += a[i] * b[i];
    }
    return result;
  }
  std::array<std::array<value_type, _n>, _m> data_;
};

struct MatrixModule {
  int start, end;
  int m, n;
};

template<int _m, int _n, int _l, class T>
Matrix_old<_m, _l, T> MultiplyMultithreaded(const Matrix_old<_m, _n, T>& first,
                                            const Matrix_old<_n, _l, T>& second,
                                            int module_size) {
  Matrix_old<_m, _l, T> result;
  int k = 5;
  return result;
}

}
#endif //THREAD__MATRIX_OLD_H_
