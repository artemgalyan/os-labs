#include <iostream>

#include "testing/MatrixTester.h"

int main() {
  MatrixTester tester(50, 50, 50);
  tester.CheckResults(true)
        .RunTests();
  return 0;
}
