cmake_minimum_required(VERSION 3.23)
project(matrix_channel_multiplication)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp matrix.h vector_wrapper.h matrix_utils.h
    testing/Timer.cpp testing/Timer.h testing/MatrixTester.cpp testing/MatrixTester.h
    matrix_utils.cpp buffered_channel.h queue_s.h)

set(CMAKE_CXX_FLAGS "-O3")