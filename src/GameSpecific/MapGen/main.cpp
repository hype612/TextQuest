#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "MapGenerator.h"
#include "bsp_node.hpp"

// TODO: remove if testing is done
std::ostream &printWithSpawns(std::ostream &os, const MapGenerator &mg) {
  std::ostringstream buf;
  buf << mg; // reuse MapGenerator's own wall/floor rendering

  std::vector<std::string> lines;
  std::string line;
  std::istringstream iss(buf.str());
  while (std::getline(iss, line)) {
    lines.push_back(line);
  }

  for (const SpawnPoint &sp : mg._spawnPoints) {
    char marker = sp.type == SpawnType::PLAYER ? 'P'
                  : sp.type == SpawnType::BOSS ? 'B'
                                               : 'E';
    lines[sp.pos.y][sp.pos.x] = marker;
  }

  constexpr const char *ANSI_RESET = "\033[0m";
  constexpr const char *ANSI_GREEN = "\033[32m";        // player
  constexpr const char *ANSI_RED = "\033[31m";          // boss
  constexpr const char *ANSI_ORANGE = "\033[38;5;208m"; // enemy

  for (const std::string &l : lines) {
    for (char c : l) {
      switch (c) {
      case 'P':
        os << ANSI_GREEN << c << ANSI_RESET;
        break;
      case 'B':
        os << ANSI_RED << c << ANSI_RESET;
        break;
      case 'E':
        os << ANSI_ORANGE << c << ANSI_RESET;
        break;
      default:
        os << c;
      }
    }
    os << '\n';
  }
  return os;
}

// TODO: drop this ASAP
int test_main() {
  std::srand(std::time(nullptr));

  part_params pp;
  Rect r{0, 0, 60, 40};

  // BspNode's constructor logs each leaf rect to std::cout as it partitions;
  // redirect that away so this program only prints the final generated map.
  std::ostringstream discard;
  std::streambuf *oldBuf = std::cout.rdbuf(discard.rdbuf());

  MapGenerator generator(r, pp, /*min_wall_neighbor_count=*/5,
                         /*noise_distribution=*/0.35f);
  std::ostringstream beforeStream;
  beforeStream << generator;
  std::string beforeLinks = beforeStream.str();
  generator.generateSpawnPoints(/*max_enemy_density=*/0.05f,
                                /*boss_room_count=*/1);

  std::cout.rdbuf(oldBuf);

  // Compare the two maps: after createLinks() the rooms should be joined and
  // every floor cell not reachable from them should have turned into a wall.
  std::cout << "--- after FillRooms ---\n" << beforeLinks;
  std::cout << "\n--- after createLinks (corridors + unreachable pruned) ---\n"
            << generator;
  std::cout << "\n--- with spawn points (P player, B boss, E enemy) ---\n";
  printWithSpawns(std::cout, generator);
}
