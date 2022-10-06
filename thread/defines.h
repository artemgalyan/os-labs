#ifndef THREAD_TESTING_DEFINES_H_
#define THREAD_TESTING_DEFINES_H_

#define VECTOR_USE_FAKE_LOCK_GUARD // significantly improves the performance, but I recommend to use it for safety
//#define ALWAYS_SHRINK_TO_FIT // don't use this

#define USE_PTRS_FOR_ROWS_AND_COLS // generally works better with that

//#define TESTER_CHECK_MULTIPLICATION_RESULTS

#endif //THREAD_TESTING_DEFINES_H_
