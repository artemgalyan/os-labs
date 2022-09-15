#include "pch.h"
#include "number.h"

namespace number {
const Number ZERO = Number(0);
const Number ONE = Number(1);

Number::Number(int value) : value_(value) {}

Number::operator int() const {
  return value_;
}

Number Number::operator+(const Number& other) const {
  return Number(value_ + other.value_);
}

Number Number::operator-(const Number& other) const {
  return Number(value_ - other.value_);
}

Number Number::operator/(const Number& other) const {
  return Number(value_ / other.value_);
}

Number Number::operator*(const Number& other) const {
  return Number(value_ * other.value_);
}

Number CreateNumber(int value) {
  return Number(value);
}

Number CreateNumber(Number from) {
  return Number(from);
}
}