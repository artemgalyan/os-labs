cmake_minimum_required(VERSION 3.23)
project(lab_5)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
  set(THREAD_CLASS threads/WinThread.h)
elseif(LINUX)
  set(THREAD_CLASS threads/LinuxThread.h threads/Thread.h)
else()
  message(FATAL_ERROR "Failed to defined OS")
endif()

add_executable(lab_5 main.cpp threads/Thread.h ${THREAD_CLASS})
