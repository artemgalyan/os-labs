#pragma once

#ifdef VECTOR_EXPORTS
#define DllExport __declspec(dllexport)
#else
#define DllExport __declspec(dllimport)
#endif

#include <number.h>

namespace vector {

class DllExport Vector {
public:
  typedef number::Number Number;
  Vector(Number x, Number y);
  Vector(const Vector&) = default;
  Vector& operator=(const Vector&) = default;
  double GetAngle() const;
  double GetRadius() const;
  Number GetX() const;
  Number GetY() const;
  Vector operator+(const Vector& v) const;
  Vector operator-() const;
private:
  Number x_;
  Number y_;
};

extern DllExport const Vector ONE_ONE;
extern DllExport const Vector ZERO_ZERO;

}