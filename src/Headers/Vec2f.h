#ifndef VEC2F_H
#define VEC2F_H

struct vec2f {
  float x, y;

  vec2f operator+(const vec2f &rval) { return {x + rval.x, y + rval.y}; }
  vec2f operator-(const vec2f &rval) { return {x - rval.x, y - rval.y}; }
  vec2f operator*(float rval) { return {x * rval, y * rval}; }
  float dot(const vec2f &rval) { return x * rval.x + y * rval.y; }
};

#endif // VEc2F_H
