#pragma once

#include "bsp_node.hpp"
#include <cstdlib>
#include <ostream>
#include <utility>
#include <vector>

enum class Tile { FLOOR, WALL };

class CellularAutomata {
public:
  // NOTE: The constructor creates the whole tree
  //       Rename?
  CellularAutomata(int min_wall_neighbor_c, float noise_distribution,
                   const Rect &area)
      : _rect(area), _min_w_nb_c(min_wall_neighbor_c),
        _noise_threshold(static_cast<int>(noise_distribution * 100)),
        _prevGen(area.h, std::vector<Tile>(area.w)),
        _currGen(area.h, std::vector<Tile>(area.w)) {
    // create min_wall_neighbor_count, noise_distribution first generation
    // (Noise)
    for (int y = 0; y < area.h; y++) {
      for (int x = 0; x < area.w; x++) {
        _currGen[y][x] =
            (rand() % 100 <= _noise_threshold) ? Tile::WALL : Tile::FLOOR;
      }
    }
  }

  void advanceGeneration() {
    // Move curr => prev
    std::swap(_currGen, _prevGen);
    // read from prev and write to curr
    for (int y = 0; y < _rect.h; y++) {
      for (int x = 0; x < _rect.w; x++) {
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

  friend std::ostream &operator<<(std::ostream &os,
                                  const CellularAutomata &ca) {
    for (const auto &row : ca._currGen) {
      for (Tile t : row) {
        os << (t == Tile::WALL ? '#' : '.');
      }
      os << '\n';
    }
    return os;
  }

  const std::vector<std::vector<Tile>> &currentGen() const { return _currGen; }

private:
  int countWallNeighbors(int x, int y) const {
    int c = 0;
    for (int rel_x = -1; rel_x <= 1; rel_x++) {
      for (int rel_y = -1; rel_y <= 1; rel_y++) {
        if (rel_x == 0 && rel_y == 0)
          continue;
        int abs_x = x + rel_x;
        int abs_y = y + rel_y;
        if (abs_x < 0 || abs_x >= _rect.w || abs_y < 0 || abs_y >= _rect.h) {
          c++;
        } else if (_prevGen[abs_y][abs_x] == Tile::WALL) {
          c++;
        }
      }
    }
    return c;
  }

  Rect _rect;
  int _min_w_nb_c;
  int _noise_threshold;
  std::vector<std::vector<Tile>> _prevGen;
  std::vector<std::vector<Tile>> _currGen;
};
