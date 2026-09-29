#include <ctime>
#include <iostream>

#include "bsp_node.hpp"
#include "bsp_visualizer.hpp"

int main() {
  std::srand(std::time(nullptr));
  part_params pp;
  Rect r{0, 0, 50, 50};
  std::cout << "x;y;w;h" << std::endl;
  BspNode root(r, nullptr, pp);
  visualize(root);
}
