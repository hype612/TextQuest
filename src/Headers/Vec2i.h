#ifndef VEC2I_H
#define VEC2I_H

struct vec2i {
  int x, y;

  vec2i operator+(const vec2i &rval) { return {x + rval.x, y + rval.y}; }
  vec2i operator-(const vec2i &rval) { return {x - rval.x, y - rval.y}; }
  bool operator==(const vec2i &rval) const {
    return x == rval.x && y == rval.y;
  }
  bool operator!=(const vec2i &rval) const { return !(*this == rval); }
  vec2i &operator+=(const vec2i &rval) {
    x += rval.x;
    y += rval.y;
    return *this;
  }
  vec2i &operator-=(const vec2i &rval) {
    x -= rval.x;
    y -= rval.y;
    return *this;
  }
  vec2i &operator*=(float rval) {
    x *= rval;
    y *= rval;
    return *this;
  }
};

#endif // VEC2I_H
