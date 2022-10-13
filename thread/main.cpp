#include <iostream>

#include "matrix.h"
#include "testing/MatrixTester.h"

int main() {
  const int size = 150;
  const int m = size;
  const int n = size;
  const int l = size;
  MatrixTester tester(m, n, l);
  tester.SetStartBlockSize(10)
        .CheckResults(true)
        .RunTests();
  return 0;
}
