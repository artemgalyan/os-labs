#include <iostream>

#include "for_each_in_input.h"

int main() {
  const int n = 7;
  ForEachInInput([](int k) {
    std::cout << k + n << ' ';
  });
  std::cout << std::endl;
  return 0;
}