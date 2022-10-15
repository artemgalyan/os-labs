cmake_minimum_required(VERSION 3.23)
project(buffered_channel)

set(CMAKE_CXX_STANDARD 23)

add_executable(buffered_channel main.cpp buffered_channel.h queue_s.h)
