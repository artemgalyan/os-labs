#ifndef THREAD__MATRIX_UTILS_H_
#define THREAD__MATRIX_UTILS_H_

#include "matrix.h"
#include <bits/std_thread.h>
#include <iostream>
#include <random>
#include "threads/Thread.h"

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
static void MultiplyBlocksAndPutInMatrix(const Matrix<T>& first,
                                         const Matrix<T>& second,
                                         Matrix<T>& result_block) {
  result_block += first * second;
}

template<MatrixType T>
std::vector<Thread> CalculateBlock(const Matrix<Matrix<T>>& a_split, const Matrix<Matrix<T>>& b_split,
                    Matrix<Matrix<T>>& result_blocks,
                    int i,
                    int j) {
  std::vector<Thread> threads;
  VectorWrapper<const Matrix<T>*> row = a_split.GetRowPtr(i);
  VectorWrapper<const Matrix<T>*> column = b_split.GetColumnPtr(j);
  result_blocks.Set(i, j,
                    Matrix<T>(row.At(0)->GetM(), column.At(0)->GetN()));
  for (int k = 0; k < row.Size(); ++k) {
    Thread tr(MultiplyBlocksAndPutInMatrix<T>,
                   std::ref(*row.At(k)),
                   std::ref(*column.At(k)),
                   std::ref(result_blocks.GetRef(i, j)));
    threads.push_back(std::move(tr));
  }
  return threads;
}

template<MatrixType T>
Matrix<T> MultiplyMultithreaded(const Matrix<T>& a, const Matrix<T>& b, int block_size = -1) {
  if (!Matrix<T>::AreMatched(a, b)) {
    throw std::invalid_argument("Matrices are not matched");
  }

  if (block_size < 1) {
    block_size = std::min({a.GetM(), a.GetN(), b.GetM()}) / 2;
  }

#if __linux__
  int _k = ceil(a.GetN() / block_size);
  if (_k >= LinuxThread::MAX_NUMBER_OF_THREADS) {
    throw std::invalid_argument("k / block_size > " + std::to_string(LinuxThread::MAX_NUMBER_OF_THREADS) +
          ", cannot multiply matrices in multithreaded mode");
  }
#endif

  Matrix<Matrix<T>> a_split = SplitIntoBlocks(a, block_size);
  Matrix<Matrix<T>> b_split = SplitIntoBlocks(b, block_size);
  Matrix<Matrix<T>> result_blocks(a_split.GetM(), b_split.GetN());
  const size_t m = result_blocks.GetM();
  const size_t n = result_blocks.GetN();
  const size_t k = a_split.GetN();
  std::vector<std::vector<Thread>> threads;
  size_t thread_count = 0;
  for (auto i = 0; i < m; ++i) {
    for (auto j = 0; j < n; ++j) {
#if __linux__ // LINUX has limit of threads
      if (thread_count + k >= LinuxThread::MAX_NUMBER_OF_THREADS) {
        for (auto& tr: threads) {
          for (auto& thread: tr) {
            thread.Join();
          }
        }
        thread_count = 0;
        threads.clear();
      }
      thread_count += k;
#endif
      auto threads_to_insert = CalculateBlock(a_split, b_split, result_blocks, i, j);
      threads.emplace_back(std::move(threads_to_insert));
    }
  }
  for (auto& tr: threads) {
    for (auto& thread: tr) {
      thread.Join();
    }
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
