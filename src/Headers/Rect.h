#ifndef RECT_H
#define RECT_H

struct Rect {
  int x;
  int y;
  int width;
  int height;

  bool overlaps(const Rect &other) const {
    return x < other.x + other.width && x + width > other.x &&
           y < other.y + other.height && y + height > other.y;
  }
};

#endif // RECT_H
