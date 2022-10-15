#ifndef VECTOR__VECTOR_H_
#define VECTOR__VECTOR_H_

#include <number.h>

namespace vector {

class Vector {
 public:
  typedef number::Number Number;
  Vector(Number x, Number y);
  Vector(const Vector&) = default;
  Vector& operator=(const Vector&) = default;
  [[nodiscard]] double GetAngle() const;
  [[nodiscard]] double GetRadius() const;
  [[nodiscard]] Number GetX() const;
  [[nodiscard]] Number GetY() const;
  Vector operator-() const;
  Vector operator+(const Vector& v) const;
 private:
  Number x_;
  Number y_;
};

extern const Vector ONE_ONE;
extern const Vector ZERO_ZERO;

}

#endif //VECTOR__VECTOR_H_
