#include <iostream>
#include <csignal>

#include "for_each_in_input.h"

int main() {
  int sum = 0;
  sleep(1);
  ForEachInInput([&sum](int k) {
    sum += k;
  });
  std::cout << sum << std::endl;
  return 0;
}