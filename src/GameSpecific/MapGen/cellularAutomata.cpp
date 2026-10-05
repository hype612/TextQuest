#include "cellularAutomata.hpp"

#include <cstdlib>
#include <ostream>
#include <utility>

CellularAutomata::CellularAutomata(int min_wall_neighbor_c,
                                   float noise_distribution,
                                   const Rect &area)
    : _rect(area), _min_w_nb_c(min_wall_neighbor_c),
      _noise_threshold(static_cast<int>(noise_distribution * 100)),
      _prevGen(area.height, std::vector<Tile>(area.width)),
      _currGen(area.height, std::vector<Tile>(area.width)) {
  // create min_wall_neighbor_count, noise_distribution first generation
  // (Noise)
  for (int y = 0; y < static_cast<int>(area.height); y++) {
    for (int x = 0; x < static_cast<int>(area.width); x++) {
      _currGen[y][x] =
          (rand() % 100 <= _noise_threshold) ? Tile::WALL : Tile::FLOOR;
    }
  }
}

void CellularAutomata::advanceGeneration() {
  // Move curr => prev
  std::swap(_currGen, _prevGen);
  // read from prev and write to curr
  for (int y = 0; y < static_cast<int>(_rect.height); y++) {
    for (int x = 0; x < static_cast<int>(_rect.width); x++) {
      // check neighbors
      if (countWallNeighbors(x, y) >= _min_w_nb_c) {
        _currGen[y][x] = Tile::WALL;
      } else {
        // Intentionally not resetting to FLOOR: leaving the stale
        // (pre-swap) value makes walls "sticky" across generations,
        // producing coherent cave blobs instead of collapsing to noise.
      }
    }
  }
}

std::ostream &operator<<(std::ostream &os, const CellularAutomata &ca) {
  for (const auto &row : ca._currGen) {
    for (Tile t : row) {
      os << (t == Tile::WALL ? '#' : '.');
    }
    os << '\n';
  }
  return os;
}

int CellularAutomata::countWallNeighbors(int x, int y) const {
  int c = 0;
  for (int rel_x = -1; rel_x <= 1; rel_x++) {
    for (int rel_y = -1; rel_y <= 1; rel_y++) {
      if (rel_x == 0 && rel_y == 0)
        continue;
      int abs_x = x + rel_x;
      int abs_y = y + rel_y;
      if (abs_x < 0 || abs_x >= static_cast<int>(_rect.width) || abs_y < 0 ||
          abs_y >= static_cast<int>(_rect.height)) {
        c++;
      } else if (_prevGen[abs_y][abs_x] == Tile::WALL) {
        c++;
      }
    }
  }
  return c;
}
