#include <ctime>
#include <iostream>
#include <sstream>

#include "MapGenerator.h"
#include "bsp_node.hpp"

int main() {
  std::srand(std::time(nullptr));

  part_params pp;
  Rect r{0, 0, 60, 40};

  // BspNode's constructor logs each leaf rect to std::cout as it partitions;
  // redirect that away so this program only prints the final generated map.
  std::ostringstream discard;
  std::streambuf *oldBuf = std::cout.rdbuf(discard.rdbuf());

  MapGenerator generator(r, pp, /*min_wall_neighbor_count=*/5,
                         /*noise_distribution=*/0.4f);
  generator.FillRooms();
  generator.createLinks();

  std::cout.rdbuf(oldBuf);

  std::cout << generator;
}
