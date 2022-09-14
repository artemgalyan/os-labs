#include "number.h"

namespace number {

const Number ZERO = Number(0);
const Number ONE = Number(1);

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

Number NumberFrom(int value) {
  return {value};
}
Number NumberFrom(Number number) {
  return {number};
}
}