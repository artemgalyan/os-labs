cmake_minimum_required(VERSION 3.23)
project(S)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp ../for_each_in_input.h ../for_each_in_input.cpp)
