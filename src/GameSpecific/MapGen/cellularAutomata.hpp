#pragma once

#include "bsp_node.hpp"
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

enum class Tile { FLOOR, WALL };

class CellularAutomata {
public:
  CellularAutomata(int min_wall_neighbor_c, float noise_distribution, Rect area)
      : _rect(area), _min_w_nb_c(min_wall_neighbor_c),
        _noise_threshold(static_cast<int>(noise_distribution * 100)),
        _prevGen(area.h, std::vector<Tile>(area.w)),
        _currGen(area.h, std::vector<Tile>(area.w)) {
    // create first generation (Noise)
    for (int y = 0; y < area.h; y++) {
      for (int x = 0; x < area.w; x++) {
        _currGen[y][x] =
            (rand() % 100 <= _noise_threshold) ? Tile::WALL : Tile ::FLOOR;
      }
    }
  }

  void advanceGeneration() {
    // Move curr => prev
    std::swap(_currGen, _prevGen);
    // read from prev and write to curr
  }

private:
  Rect _rect;
  int _min_w_nb_c;
  int _noise_threshold;
  std::vector<std::vector<Tile>> _prevGen;
  std::vector<std::vector<Tile>> _currGen;
};
