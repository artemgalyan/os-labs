#include <iostream>

#include "matrix.h"
#include "testing/MatrixTester.h"

int main() {
  const int m = 18;
  const int n = 15;
  const int l = 40;
  MatrixTester tester(m, n, l);
  tester.RunTests();
  return 0;
}
