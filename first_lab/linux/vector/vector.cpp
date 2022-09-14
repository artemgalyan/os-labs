#include "vector.h"

#include <cmath>
#include <numbers>

using number::Number;

const Vector Vector::ZERO_ZERO = Vector(number::ZERO, number::ZERO);
const Vector Vector::ONE_ONE = Vector(number::ONE, number::ONE);

Vector::Vector(Number x, Number y) : x_(x), y_(y) {}

double Vector::GetAngle() const {
  int y = static_cast<int>(y_);
  int x = static_cast<int>(x_);
  double angle = atan((double) y / x);
  if (x_ >= 0) {
    return angle;
  }
  if (y_ >= 0) {
    return std::numbers::pi - angle;
  }
  return std::numbers::pi + angle;
}

double Vector::GetRadius() const {
  return sqrt(static_cast<int>(x_ * x_ + y_ * y_));
}

Number Vector::GetX() const {
  return x_;
}

Number Vector::GetY() const {
  return y_;
}
