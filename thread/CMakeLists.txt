cmake_minimum_required(VERSION 3.23)
project(thread)

set(CMAKE_CXX_STANDARD 20)

add_executable(thread main.cpp Matrix_old.h matrix.h vector_wrapper.h matrix_utils.h testing/Timer.cpp testing/Timer.h testing/MatrixTester.cpp testing/MatrixTester.h number_matrix.h matrix_utils.cpp)
