#include "for_each_in_input.h"

void ForEachInInput(const std::function<void(int)>& function) {
  std::string line;
  std::getline(std::cin, line);
  std::stringstream ss(line);
  int n;
  while (ss >> n) {
    function(n);
  }
}