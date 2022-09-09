#pragma once

#include <compare>

class Number
{
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

