#include <iostream>

#include "testing/MatrixTester.h"

int main() {
  MatrixTester tester(50, 50, 50);
  tester.RunTests();
  return 0;
}
