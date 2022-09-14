#include <iostream>
#include <number.h>
#include <vector.h>

std::ostream& operator<<(std::ostream& out, const vector::Vector& vector) {
  return out << '(' << vector.GetX() << ", " << vector.GetY() << ')';
}

int main() {
  using vector::Vector;
  using number::Number;
  Number one = number::CreateNumber(1);
  Number two = number::CreateNumber(2);
  Number three = number::CreateNumber(two + one);
  auto value = one + two / three + two * two - one;
  std::cout << one << " " << two << " " << value << " " << typeid(value).name() << std::endl;

  Vector v1(one, two);
  Vector v2 = vector::ONE_ONE;
  std::cout << v1 + v2 << ", " << -(v1 + v2);
  return 0;
}