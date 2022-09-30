#ifndef THREAD__MATRIX_UTILS_H_
#define THREAD__MATRIX_UTILS_H_

#include "matrix.h"

#include <iostream>
#include <random>

namespace matrix {

template<MatrixType T>
Matrix<T> SubmatrixOf(const Matrix<T>& matrix, int start_pos_i, int start_pos_j, int m, int n) {
  Matrix<T> result(m, n);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      result.Set(i, j, matrix.Get(i + start_pos_i, j + start_pos_j));
    }
  }
  return result;
}

template<MatrixType T>
void InsertSubmatrix(Matrix<T>& matrix, const Matrix<T>& to_insert, int start_i, int start_j) {
  int m = to_insert.GetM();
  int n = to_insert.GetN();
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      T value = to_insert.Get(i, j);
      matrix.Set(i + start_i, j + start_j, value);
    }
  }
}

template<MatrixType T>
Matrix<Matrix<T>> SplitIntoBlocks(const Matrix<T>& matrix, int block_size) {
  int m = ceil((double) matrix.GetM() / block_size);
  int n = ceil((double) matrix.GetN() / block_size);
  Matrix<Matrix<T>> result(m, n);
  for (int i = 0; i < m; ++i) {
    int block_height = std::min(matrix.GetM() - block_size * i, block_size);
    for (int j = 0; j < n; ++j) {
      int block_width = std::min(matrix.GetN() - block_size * j, block_size);
      result.Set(i, j,
                 SubmatrixOf(matrix, i * block_size, j * block_size, block_height, block_width));
    }
  }
  return result;
}

template<MatrixType T>
Matrix<T> JoinIntoMatrix(const Matrix<Matrix<T>>& matrix, int m, int n) {
  Matrix<T> result(m, n);
  int offset_i = 0;
  int to_m = matrix.GetM(), to_n = matrix.GetN();
  for (int i = 0; i < to_m; ++i) {
    int offset_j = 0;
    for (int j = 0; j < to_n; ++j) {
      InsertSubmatrix(result, matrix.Get(i, j), offset_i, offset_j);
      offset_j += matrix.Get(i, j).GetN();
    }
    offset_i += matrix.Get(i, 0).GetM();
  }
  return result;
}

template<MatrixType T>
Matrix<T> MultiplyMultithreaded(const Matrix<T>& a, const Matrix<T>& b, int block_size = -1) {
  if (!Matrix<T>::AreMatched(a, b)) {
    throw std::invalid_argument("Matrices are not matched");
  }

  if (block_size < 1) {
    block_size = std::min({a.GetM(), a.GetN(), b.GetM()}) / 2;
  }
  Matrix<Matrix<T>> a_split = SplitIntoBlocks(a, block_size);
  Matrix<Matrix<T>> b_split = SplitIntoBlocks(b, block_size);
  Matrix<Matrix<T>> result_blocks(a_split.GetM(), b_split.GetN());
  std::vector<std::thread> threads;
  for (int i = 0; i < result_blocks.GetM(); ++i) {
    for (int j = 0; j < result_blocks.GetN(); ++j) {
      std::thread thread(
          [&a_split, &b_split, &result_blocks, i, j]() {
            int m = a_split.Get(i, 0).GetM();
            int n = b_split.Get(0, j).GetN();

            Matrix<T> matrix(m, n);
            auto row = a_split.GetRow(i);
            auto col = b_split.GetColumn(j);
            for (int l = 0; l < row.Size(); ++l) {
              matrix += row.At(l) * col.At(l);
            }
            result_blocks.Set(i, j, matrix);
          });
      threads.push_back(std::move(thread));
    }
  }
  for (auto& thread : threads) {
    thread.join();
  }
  return JoinIntoMatrix(result_blocks, a.GetM(), b.GetN());
}

template<class T>
void Print(const matrix::Matrix<T>& matrix) {
  int m = matrix.GetM();
  int n = matrix.GetN();
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << matrix.Get(i, j) << " ";
    }
    std::cout << std::endl;
  }
}

Matrix<int> GenerateMatrix(int m,
                           int n,
                           int min_value = std::numeric_limits<int>::min(),
                           int max_value = std::numeric_limits<int>::max());

}

#endif //THREAD__MATRIX_UTILS_H_
