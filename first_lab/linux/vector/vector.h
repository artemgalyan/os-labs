#ifndef VECTOR__VECTOR_H_
#define VECTOR__VECTOR_H_

#include <number.h>

class Vector {
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

#endif //VECTOR__VECTOR_H_
