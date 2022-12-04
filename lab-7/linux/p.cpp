#include <iostream>

#include "for_each_in_input.h"

int main() {
  ForEachInInput([](int k) {
    std::cout << k * k * k << ' ';
  });
  std::cout << std::endl;
  return 0;
}