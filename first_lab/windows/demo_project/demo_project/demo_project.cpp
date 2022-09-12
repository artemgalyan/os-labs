#include <iostream>
#include <number.h>
#include <vector.h>

std::ostream& operator<<(std::ostream& out, const Vector& vector) {
  return out << '(' << vector.GetX() << ", " << vector.GetY() << ')';
}

int main() {
  using number::Number;
  auto one = Number::ONE;
  auto two = number::CreateNumber(2);
  auto three = number::CreateNumber(two + one);
  Number sum = one + two + three;
  int sum_as_int = sum;
  std::cout << one << " " << two << " " << sum << " " << sum_as_int << std::endl;

  Vector v1(one, two);
  Vector v2 = Vector::ONE_ONE;
  std::cout << v1 + v2 << ", " << -(v1 + v2);
  return 0;
}