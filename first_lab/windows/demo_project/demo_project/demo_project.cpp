#include <iostream>
#include <number.h>
#include <vector.h>

std::ostream& operator<<(std::ostream& out, const Vector& vector) {
  return out << '(' << vector.GetX() << ", " << vector.GetY() << ')';
}

int main() {
  auto one = Number::ONE;
  auto zero = Number::ZERO;
  auto ten = Number(10);
  Number result = 5 * one + 19 * zero + ten + 1;
  int intValue = result;
  std::cout << result << " " << intValue << std::endl;
  Vector v = Vector::ZERO_ZERO;
  std::cout << v << std::endl;
  v = Vector(result, 10);
  std::cout << v << std::endl;
  return 0;
}