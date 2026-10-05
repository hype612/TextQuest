#include "bsp_node.hpp"

#include <cstdlib>
#include <iostream>

int partition_delim(int l, int h) { return rand() % (h - l) + l; }

std::ostream &operator<<(std::ostream &o, const Rect &r) {
  return o << r.x << ";" << r.y << ";" << r.width << ";" << r.height;
}

BspNode::BspNode(Rect r, BspNode *parent, const part_params &pp)
    : _parent(parent), _rect(r) {
  // Partition if possible
  // if not == leaf node so print
  if ((static_cast<int>(_rect.width) > pp.max_width ||
       static_cast<int>(_rect.height) > pp.max_height) &&
      (static_cast<int>(_rect.width) >= pp.min_width &&
       static_cast<int>(_rect.height) >= pp.min_height))
    partition(pp);
  else {
    std::cout << _rect << std::endl;
  }
}

void BspNode::partition(const part_params &pp) {
  split s = static_cast<split>(rand() % 2);
  if (s == split::HORIZONTAL) {
    int delim =
        partition_delim(static_cast<int>(_rect.width * pp.part_low_bound),
                        static_cast<int>(_rect.width * pp.part_high_bound));
    _l_child = std::make_unique<BspNode>(
        Rect{_rect.x, _rect.y, static_cast<unsigned int>(delim), _rect.height},
        this, pp);
    _r_child = std::make_unique<BspNode>(
        Rect{_rect.x + delim, _rect.y,
             _rect.width - static_cast<unsigned int>(delim), _rect.height},
        this, pp);
  } else {
    int delim =
        partition_delim(static_cast<int>(_rect.height * pp.part_low_bound),
                        static_cast<int>(_rect.height * pp.part_high_bound));
    _l_child = std::make_unique<BspNode>(
        Rect{_rect.x, _rect.y, _rect.width, static_cast<unsigned int>(delim)},
        this, pp);
    _r_child = std::make_unique<BspNode>(
        Rect{_rect.x, _rect.y + delim, _rect.width,
             _rect.height - static_cast<unsigned int>(delim)},
        this, pp);
  }
}
