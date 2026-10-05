#pragma once

#include <iosfwd>
#include <memory>

#include "../../Headers/Rect.h"

int partition_delim(int l, int h);

std::ostream &operator<<(std::ostream &o, const Rect &r);

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
  BspNode(Rect r, BspNode *parent, const part_params &pp);
  void partition(const part_params &pp);

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
