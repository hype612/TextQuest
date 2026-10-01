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
                         /*noise_distribution=*/0.35f);
  generator.FillRooms();
  std::ostringstream beforeStream;
  beforeStream << generator;
  std::string beforeLinks = beforeStream.str();
  generator.createLinks();

  std::cout.rdbuf(oldBuf);

  // Compare the two maps: after createLinks() the rooms should be joined and
  // every floor cell not reachable from them should have turned into a wall.
  std::cout << "--- after FillRooms ---\n" << beforeLinks;
  std::cout << "\n--- after createLinks (corridors + unreachable pruned) ---\n"
            << generator;
}
