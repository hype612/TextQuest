#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "../../Headers/Vec2i.h"
#include "bsp_node.hpp"
#include "cellularAutomata.hpp"
#include <queue>

constexpr int ideal_generation_num = 4;

class MapGenerator {
public:
  MapGenerator(const Rect &parent_area, const part_params &params,
               int min_wall_neighbor_count, float noise_distribution)
      : _bspRoot(parent_area, nullptr, params),
        _ca_min_wall_neighbor_count(min_wall_neighbor_count),
        _ca_noise_distribution(noise_distribution), _parentArea(parent_area),
        _map(parent_area.h, std::vector<Tile>(parent_area.w, Tile::WALL)) {}
  // fill the rects created by BSP
  void FillRooms() {
    // traverse bsp.
    // if leaf == let generator do its thang
    createRoom(&_bspRoot);
    enforceBorder();
  }

  // create walkable links between rooms, then fill in any floor
  // that can't be reached from them (isolated cave pockets)
  void createLinks() {
    vec2i bfs_start = connectSubtree(&_bspRoot);
    std::vector<std::vector<bool>> accessible_mask = floodfill(bfs_start);
    for (size_t y = 0; y < _map.size(); y++) {
      for (size_t x = 0; x < _map[y].size(); x++) {
        if (_map[y][x] == Tile::FLOOR && !accessible_mask[y][x]) {
          _map[y][x] = Tile::WALL;
        }
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const MapGenerator &mg) {
    for (const auto &row : mg._map) {
      for (Tile t : row) {
        os << (t == Tile::WALL ? '#' : '.');
      }
      os << '\n';
    }
    return os;
  }

private:
  std::vector<std::vector<bool>> floodfill(vec2i bfs_start) {
    std::vector<std::vector<bool>> msk(
        _map.size(), std::vector<bool>(_map[0].size(), false));
    std::queue<vec2i> q;
    q.push(bfs_start);
    msk[bfs_start.y][bfs_start.x] = true;

    while (!q.empty()) {
      vec2i nxt = q.front();
      q.pop();

      if (nxt.y > 0 && msk[nxt.y - 1][nxt.x] == false &&
          _map[nxt.y - 1][nxt.x] == Tile::FLOOR) {
        msk[nxt.y - 1][nxt.x] = true;
        q.push({nxt.x, nxt.y - 1});
      }
      if (static_cast<unsigned int>(nxt.y + 1) < _map.size() &&
          msk[nxt.y + 1][nxt.x] == false &&
          _map[nxt.y + 1][nxt.x] == Tile::FLOOR) {
        msk[nxt.y + 1][nxt.x] = true;
        q.push({nxt.x, nxt.y + 1});
      }
      if (nxt.x > 0 && msk[nxt.y][nxt.x - 1] == false &&
          _map[nxt.y][nxt.x - 1] == Tile::FLOOR) {
        msk[nxt.y][nxt.x - 1] = true;
        q.push({nxt.x - 1, nxt.y});
      }
      if (static_cast<unsigned int>(nxt.x + 1) < _map[0].size() &&
          msk[nxt.y][nxt.x + 1] == false &&
          _map[nxt.y][nxt.x + 1] == Tile::FLOOR) {
        msk[nxt.y][nxt.x + 1] = true;
        q.push({nxt.x + 1, nxt.y});
      }
    }
    return msk;
  }

  void createRoom(const BspNode *node) {
    if (!node) {
      return;
    }

    if (node->is_leaf()) {
      CellularAutomata ca(_ca_min_wall_neighbor_count, _ca_noise_distribution,
                          node->rect());
      for (int i = 0; i <= ideal_generation_num; i++) {
        ca.advanceGeneration();
      }
      const Rect &r = node->rect();
      const std::vector<std::vector<Tile>> &gen = ca.currentGen();
      for (int y = 0; y < r.h; y++) {
        for (int x = 0; x < r.w; x++) {
          _map[r.y + y][r.x + x] = gen[y][x];
        }
      }
    }

    createRoom(node->left());
    createRoom(node->right());
  }

  // Post-order: connect both children first, then link this node's two
  // subtrees together. Returns a floor-cell anchor point somewhere in the
  // now-fully-connected subtree, for the parent to link into.
  vec2i connectSubtree(const BspNode *node) {
    if (node->is_leaf()) {
      vec2i floorcell;
      while ((floorcell = findFloorCell(node->rect())) == vec2i{-1, -1}) {
        // room is too dense - recreate
        CellularAutomata ca(_ca_min_wall_neighbor_count, _ca_noise_distribution,
                            node->rect());
        for (int i = 0; i <= ideal_generation_num; i++) {
          ca.advanceGeneration();
        }
        const Rect &r = node->rect();
        const std::vector<std::vector<Tile>> &gen = ca.currentGen();
        for (int y = 0; y < r.h; y++) {
          for (int x = 0; x < r.w; x++) {
            _map[r.y + y][r.x + x] = gen[y][x];
          }
        }
      }
      return floorcell;
    }
    vec2i left = connectSubtree(node->left());
    vec2i right = connectSubtree(node->right());
    carveCorridor(left, right);
    return left;
  }

  vec2i findFloorCell(const Rect &r) {
    for (int y = 0; y < r.h; y++) {
      for (int x = 0; x < r.w; x++) {
        if (_map[r.y + y][r.x + x] == Tile::FLOOR) {
          return {r.x + x, r.y + y};
        }
      }
    }
    return {-1, -1};
  }

  // Bulldozes an L-shaped, one-tile-wide path between two floor cells:
  // horizontal run at a's row, then vertical run at b's column.
  void carveCorridor(vec2i a, vec2i b) {
    int x = a.x;
    int y = a.y;
    int xStep = (b.x > x) ? 1 : -1;
    while (x != b.x) {
      _map[y][x] = Tile::FLOOR;
      x += xStep;
    }
    int yStep = (b.y > y) ? 1 : -1;
    while (y != b.y) {
      _map[y][x] = Tile::FLOOR;
      y += yStep;
    }
    _map[b.y][b.x] = Tile::FLOOR;
  }

  void enforceBorder() {
    int h = static_cast<int>(_map.size());
    int w = h > 0 ? static_cast<int>(_map[0].size()) : 0;
    for (int x = 0; x < w; x++) {
      _map[0][x] = Tile::WALL;
      _map[h - 1][x] = Tile::WALL;
    }
    for (int y = 0; y < h; y++) {
      _map[y][0] = Tile::WALL;
      _map[y][w - 1] = Tile::WALL;
    }
  }

  BspNode _bspRoot;
  int _ca_min_wall_neighbor_count;
  float _ca_noise_distribution;
  // x and y param are pretty much meaningless here
  // only keeping it like this for consistency
  Rect _parentArea;
  std::vector<std::vector<Tile>> _map;
};

#endif // !MAPGENERATOR_H
