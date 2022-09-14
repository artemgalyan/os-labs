#pragma once

#include <compare>

namespace number {

class Number {
 public:
  Number(int value);
  Number(const Number&) = default;
  Number& operator=(const Number&) = default;
  explicit operator int() const;
  Number operator+(const Number&) const;
  Number operator-(const Number&) const;
  Number operator/(const Number&) const;
  Number operator*(const Number&) const;
  auto operator<=>(const Number&) const = default;
 private:
  int value_;
};

extern const Number ZERO;
extern const Number ONE;
Number NumberFrom(int value);
Number NumberFrom(Number number);
}