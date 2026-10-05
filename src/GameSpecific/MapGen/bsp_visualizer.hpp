#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "bsp_node.hpp"

// Renders the tree rooted at `root` as an ASCII-art grid, drawing the
// border of every leaf rectangle. Since leaves tile the root rect without
// gaps or overlaps, the shared edges between siblings naturally show up
// as the partition lines.
inline void visualize(const BspNode &root, std::ostream &os = std::cout) {
  const Rect &bounds = root.rect();

  // Terminal characters are roughly twice as tall as they are wide, so
  // stretch the x axis to keep leaves looking roughly proportional.
  const int scale_x = 2;
  const int scale_y = 1;

  const int cols = static_cast<int>(bounds.width) * scale_x + 1;
  const int rows = static_cast<int>(bounds.height) * scale_y + 1;
  std::vector<std::string> grid(rows, std::string(cols, ' '));

  auto to_col = [&](int x) { return (x - bounds.x) * scale_x; };
  auto to_row = [&](int y) { return (y - bounds.y) * scale_y; };

  auto draw_rect = [&](const Rect &r) {
    int c0 = to_col(r.x);
    int c1 = to_col(r.x + static_cast<int>(r.width));
    int r0 = to_row(r.y);
    int r1 = to_row(r.y + static_cast<int>(r.height));

    for (int c = c0; c <= c1; ++c) {
      grid[r0][c] = '-';
      grid[r1][c] = '-';
    }
    for (int row = r0; row <= r1; ++row) {
      grid[row][c0] = '|';
      grid[row][c1] = '|';
    }
    grid[r0][c0] = '+';
    grid[r0][c1] = '+';
    grid[r1][c0] = '+';
    grid[r1][c1] = '+';
  };

  std::vector<const BspNode *> stack{&root};
  while (!stack.empty()) {
    const BspNode *node = stack.back();
    stack.pop_back();
    if (node->is_leaf()) {
      draw_rect(node->rect());
      continue;
    }
    if (node->left())
      stack.push_back(node->left());
    if (node->right())
      stack.push_back(node->right());
  }

  for (const auto &line : grid)
    os << line << '\n';
}
