#pragma once

#include <compare>

namespace number {
class Number {
 public:
  const static Number ONE;
  const static Number ZERO;
  Number(int value);
  Number(const Number&) = default;
  Number& operator=(const Number&) = default;
  operator int() const;
  Number operator+(const Number&) const;
  Number operator-(const Number&) const;
  Number operator/(const Number&) const;
  Number operator*(const Number&) const;
  auto operator<=>(const Number&) const = default;
 private:
  int value_;
};
Number CreateNumber(int value);
Number CreateNumber(Number from);
}