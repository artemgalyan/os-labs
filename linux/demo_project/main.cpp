#include <iostream>
#include <number.h>
#include <vector.h>

std::ostream& operator<<(std::ostream& out, const vector::Vector& vector) {
  return out << '(' << vector.GetX() << ", " << vector.GetY() << ')';
}

int main() {
  using vector::Vector;
  using number::Number;
  Number one = number::NumberFrom(1);
  Number two = number::NumberFrom(2);
  Number three = number::NumberFrom(two + one);
  auto value = one + two / three + two * two - one;
  std::cout << one << " " << two << " " << value << " " << typeid(value).name() << std::endl;

  Vector v1(one, two);
  Vector v2 = vector::ONE_ONE;
  std::cout << v1 + v2 << ", " << -(v1 + v2) << " " << v1.GetAngle() << " " << v1.GetRadius();
  return 0;
}