#include <iostream>

#include "matrix.h"
#include "testing/MatrixTester.h"

int main() {
  const int size = 500;

  const int m = size;
  const int n = size;
  const int l = size;
  MatrixTester tester(m, n, l, ceil(size / 3.0));
  tester.SetStartThreadsCount(18)
        .CheckResults(false)
        .RunTests();
  return 0;
}
