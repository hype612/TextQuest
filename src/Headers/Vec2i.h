#ifndef VEC2I_H
#define VEC2I_H

struct vec2i {
  int x, y;

  vec2i operator+(const vec2i &rval) { return {x + rval.x, y + rval.y}; }
  vec2i operator-(const vec2i &rval) { return {x - rval.x, y - rval.y}; }
  bool operator==(const vec2i &rval) { return x == rval.x && y == rval.y; }
};

#endif // VEC2I_H
