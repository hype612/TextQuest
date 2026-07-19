#ifndef VEC2F_H
#define VEC2F_H

struct vec2f {
  float x, y;

  vec2f operator+(const vec2f &rval) const { return {x + rval.x, y + rval.y}; }
  vec2f operator-(const vec2f &rval) const { return {x - rval.x, y - rval.y}; }
  vec2f operator*(float rval) const { return {x * rval, y * rval}; }
  float dot(const vec2f &rval) const { return x * rval.x + y * rval.y; }
  vec2f &operator+=(const vec2f &rval) {
    x += rval.x;
    y += rval.y;
    return *this;
  }
  vec2f &operator-=(const vec2f &rval) {
    x -= rval.x;
    y -= rval.y;
    return *this;
  }
  vec2f &operator*=(float rval) {
    x *= rval;
    y *= rval;
    return *this;
  }
};

#endif // VEc2F_H
