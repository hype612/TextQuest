#ifndef VEC2F_H
#define VEC2F_H

#include <cmath>
#include <iostream>

struct vec2f {
  float x, y;

  vec2f operator+(const vec2f &rval) const { return {x + rval.x, y + rval.y}; }
  vec2f operator-(const vec2f &rval) const { return {x - rval.x, y - rval.y}; }
  vec2f operator*(float rval) const { return {x * rval, y * rval}; }
  float dot(const vec2f &rval) const { return x * rval.x + y * rval.y; }
  float cross(const vec2f &rval) const { return x * rval.y - y * rval.x; }
  bool operator==(const vec2f &rval) const {
    constexpr float epsilon = 1e-5f;
    return std::abs(x - rval.x) < epsilon && std::abs(y - rval.y) < epsilon;
  }
  bool operator!=(const vec2f &rval) const { return !(*this == rval); }
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
  void normalize() {
    float len = std::sqrt(x * x + y * y);
    if (len == 0) {
      std::cerr << "normalize was called on a null-vec." << std::endl;
      return;
    }
    x /= len;
    y /= len;
  }
  vec2f normalized() {
    float len = std::sqrt(x * x + y * y);
    if (len == 0) {
      std::cerr << "normalize was called on a null-vec." << std::endl;
      return {-1.f, -1.f};
    }
    return {x /= len, y /= len};
  }
};

#endif // VEC2F_H
