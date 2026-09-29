#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>

inline int partition_delim(int l, int h) { return rand() % (h - l) + l; }

struct Rect {
  int x;
  int y;
  int w;
  int h;
};

inline std::ostream &operator<<(std::ostream &o, const Rect &r) {
  return o << r.x << ";" << r.y << ";" << r.w << ";" << r.h;
}

struct part_params {
  float part_low_bound = 0.3f;
  float part_high_bound = 0.7f;
  int min_width = 10;
  int min_height = 10;
  int max_width = 20;
  int max_height = 20;
};

enum split { HORIZONTAL = 0, VERTICAL = 1 };

class BspNode {
public:
  BspNode(Rect r, BspNode *parent, const part_params &pp)
      : _parent(parent), _rect(r) {
    // Partition if possible
    // if not == leaf node so print
    if ((_rect.w > pp.max_width || _rect.h > pp.max_height) &&
        (_rect.w >= pp.min_width && _rect.h >= pp.min_height))
      partition(pp);
    else {
      std::cout << _rect << std::endl;
    }
  }
  void partition(const part_params &pp) {

    split s = static_cast<split>(rand() % 2);
    if (s == split::HORIZONTAL) {
      int delim =
          partition_delim(static_cast<int>(_rect.w * pp.part_low_bound),
                          static_cast<int>(_rect.w * pp.part_high_bound));
      _l_child = std::make_unique<BspNode>(
          Rect{_rect.x, _rect.y, delim, _rect.h}, this, pp);
      _r_child = std::make_unique<BspNode>(
          Rect{_rect.x + delim, _rect.y, _rect.w - delim, _rect.h}, this, pp);
    } else {
      int delim =
          partition_delim(static_cast<int>(_rect.h * pp.part_low_bound),
                          static_cast<int>(_rect.h * pp.part_high_bound));
      _l_child = std::make_unique<BspNode>(
          Rect{_rect.x, _rect.y, _rect.w, delim}, this, pp);
      _r_child = std::make_unique<BspNode>(
          Rect{_rect.x, _rect.y + delim, _rect.w, _rect.h - delim}, this, pp);
    }
  }

  const Rect &rect() const { return _rect; }
  const BspNode *left() const { return _l_child.get(); }
  const BspNode *right() const { return _r_child.get(); }
  bool is_leaf() const { return !_l_child && !_r_child; }

private:
  BspNode *_parent = nullptr;
  std::unique_ptr<BspNode> _l_child;
  std::unique_ptr<BspNode> _r_child;
  Rect _rect;
};
