#pragma once

#include <compare>

namespace number {
class Number {
 public:
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

extern const Number ONE;
extern const Number ZERO;
Number CreateNumber(int value);
Number CreateNumber(Number from);
}