#ifndef LAB_5_THREADS_THREAD_H_
#define LAB_5_THREADS_THREAD_H_

#ifdef WIN32
#include "WinThread.h"
  typedef WinThread Thread;
#elif __linux__
#include "LinuxThread.h"
  typedef LinuxThread Thread;
#endif

#endif //LAB_5_THREADS_THREAD_H_
