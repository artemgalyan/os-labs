#include <iostream>

#include "matrix.h"
#include "testing/MatrixTester.h"
#include "matrix_utils.h"

int main() {
  const int m = 20;
  const int n = 40;
  const int l = 20;
  MatrixTester tester(m, n, l);
  tester.RunTests();
  return 0;
}
