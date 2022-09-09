#pragma once

#ifdef VECTOR_EXPORTS
#define DllExport __declspec(dllexport)
#else
#define DllExport __declspec(dllimport)
#endif

#include <Number.h>
#include <iostream>

class DllExport Vector
{
public:
	const static Vector ZERO_ZERO;
	const static Vector ONE_ONE;
	Vector(Number x, Number y);
	Vector(const Vector&) = default;
	Vector& operator=(const Vector&) = default;
	double GetAngle() const;
	double GetRadius() const;
	Number GetX() const;
	Number GetY() const;
private:
	Number x_;
	Number y_;
};

