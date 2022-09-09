#include "number.h"

const Number Number::ZERO = Number(0);
const Number Number::ONE = Number(1);

Number::Number(int value) : value_(value) {}

Number::operator int() const {
  return value_;
}

Number Number::operator+(const Number& other) const {
  return {value_ + other.value_};
}

Number Number::operator-(const Number& other) const {
  return {value_ - other.value_};
}

Number Number::operator/(const Number& other) const {
  return {value_ / other.value_};
}

Number Number::operator*(const Number& other) const {
  return {value_ * other.value_};
}
