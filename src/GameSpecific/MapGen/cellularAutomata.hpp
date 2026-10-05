#pragma once

#include "bsp_node.hpp"
#include <iosfwd>
#include <vector>

enum class Tile { FLOOR, WALL };

class CellularAutomata {
public:
  // NOTE: The constructor creates the whole tree
  //       Rename?
  CellularAutomata(int min_wall_neighbor_c, float noise_distribution,
                   const Rect &area);

  void advanceGeneration();

  friend std::ostream &operator<<(std::ostream &os,
                                  const CellularAutomata &ca);

  const std::vector<std::vector<Tile>> &currentGen() const { return _currGen; }

private:
  int countWallNeighbors(int x, int y) const;

  Rect _rect;
  int _min_w_nb_c;
  int _noise_threshold;
  std::vector<std::vector<Tile>> _prevGen;
  std::vector<std::vector<Tile>> _currGen;
};
