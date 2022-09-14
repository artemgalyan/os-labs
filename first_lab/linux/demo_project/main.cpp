#include <iostream>
#include <number.h>
#include <vector.h>

std::ostream& operator<<(std::ostream& out, const Vector& vector) {
  return out << '(' << static_cast<int>(vector.GetX()) << ", " << static_cast<int>(vector.GetY()) << ')';
}

int main() {
  using number::Number;
  auto one = number::ONE;
  auto zero = number::ZERO;
  auto ten = Number(10);
  Number result = one * 5 + zero * 19+ ten + 1;
  int intValue = static_cast<int>(result);
  std::cout << static_cast<int>(result) << " " << intValue << std::endl;
  Vector v = Vector::ZERO_ZERO;
  std::cout << v << std::endl;
  v = Vector(result, 10);
  std::cout << v << std::endl;
  return 0;
}