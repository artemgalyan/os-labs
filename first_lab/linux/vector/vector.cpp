#include "vector.h"

#include <cmath>
#include <numbers>

const Vector Vector::ZERO_ZERO = Vector(Number::ZERO, Number::ZERO);
const Vector Vector::ONE_ONE = Vector(Number::ONE, Number::ONE);

Vector::Vector(Number x, Number y) : x_(x), y_(y) {}

double Vector::GetAngle() const
{
  double angle = atan((double)y_ / x_);
  if (x_ >= Number::ZERO) {
    return angle;
  }
  if (y_ >= Number::ZERO) {
    return std::numbers::pi - angle;
  }
  return std::numbers::pi + angle;
}

double Vector::GetRadius() const
{
  return sqrt((int)x_ * x_ + y_ * y_);
}

Number Vector::GetX() const
{
  return x_;
}

Number Vector::GetY() const
{
  return y_;
}
