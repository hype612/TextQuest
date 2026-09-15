#ifndef RECT_H
#define RECT_H

struct Rect {
  int x;
  int y;
  unsigned int width;
  unsigned int height;

  bool overlaps(const Rect &other) const {
    return x < other.x + static_cast<int>(other.width) &&
           x + static_cast<int>(width) > other.x &&
           y < other.y + static_cast<int>(other.height) &&
           y + static_cast<int>(height) > other.y;
  }
};

#endif // RECT_H
