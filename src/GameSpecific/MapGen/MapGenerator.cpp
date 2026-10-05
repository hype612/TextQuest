#include "MapGenerator.h"
#include "bsp_node.hpp"

#include <algorithm>
#include <ostream>
#include <queue>

MapGenerator::MapGenerator(const Rect &parent_area, const part_params &params,
                           int min_wall_neighbor_count,
                           float noise_distribution)
    : _bspRoot(parent_area, nullptr, params),
      _ca_min_wall_neighbor_count(min_wall_neighbor_count),
      _ca_noise_distribution(noise_distribution), _parentArea(parent_area),
      _map(parent_area.height,
           std::vector<Tile>(parent_area.width, Tile::WALL)) {}

void MapGenerator::FillRooms() {
  // traverse bsp.
  // if leaf == let generator do its thang
  createRoom(&_bspRoot);
  enforceBorder();
  FillRooms();
  createLinks();
}

void MapGenerator::createLinks() {
  vec2i bfs_start = connectSubtree(&_bspRoot);
  _accessibleMask = floodfill(bfs_start);
  for (size_t y = 0; y < _map.size(); y++) {
    for (size_t x = 0; x < _map[y].size(); x++) {
      if (_map[y][x] == Tile::FLOOR && _accessibleMask[y][x] == false) {
        _map[y][x] = Tile::WALL;
      }
    }
  }
}

std::vector<vec2f>
MapGenerator::generatePatrolPoints(vec2f spawn_point,
                                   unsigned int count) const {
  const BspNode *room = findLeafContaining(&_bspRoot, spawn_point);
  const Rect &r = room->rect();
  vec2i min{r.x, r.y};
  vec2i max{r.x + static_cast<int>(r.width), r.y + static_cast<int>(r.height)};
  std::vector<vec2f> patrol;
  patrol.reserve(count);
  while (patrol.size() != count) {
    vec2f p{0, 0};
    do {
      p = {static_cast<float>(rand() % (max.x - min.x) + min.x),
           static_cast<float>(rand() % (max.y - min.y) + min.y)};
    } while (_accessibleMask[p.y][p.x] == false);
    patrol.push_back(p);
  }

  return patrol;
}

std::vector<vec2i> MapGenerator::accessibleFloorCells(const Rect &r) const {
  std::vector<vec2i> out;
  for (int y = 0; y < static_cast<int>(r.height); y++) {
    for (int x = 0; x < static_cast<int>(r.width); x++) {
      int gx = r.x + x, gy = r.y + y;
      if (_map[gy][gx] == Tile::FLOOR && _accessibleMask[gy][gx]) {
        out.push_back({gx, gy});
      }
    }
  }
  return out;
}

void MapGenerator::generateSpawnPoints(float max_enemy_density,
                                       int boss_room_count) {
  _spawnPoints.clear();
  std::vector<Rect> available = _leafRooms;

  int playerIdx = rand() % available.size();
  auto playerCells = accessibleFloorCells(available[playerIdx]);
  _spawnPoints.push_back(
      {SpawnType::PLAYER, playerCells[rand() % playerCells.size()]});
  available.erase(available.begin() + playerIdx);

  int bossCount = std::min<int>(boss_room_count, available.size());
  for (int i = 0; i < bossCount; i++) {
    int idx = rand() % available.size();
    auto cells = accessibleFloorCells(available[idx]);
    _spawnPoints.push_back({SpawnType::BOSS, cells[rand() % cells.size()]});
    available.erase(available.begin() + idx);
  }

  for (const Rect &room : available) {
    for (vec2i cell : accessibleFloorCells(room)) {
      if ((rand() % 10000) / 10000.0f < max_enemy_density) {
        _spawnPoints.push_back({SpawnType::ENEMY, cell});
      }
    }
  }
}

// TODO: remove if testing is done
std::ostream &operator<<(std::ostream &os, const MapGenerator &mg) {
  for (const auto &row : mg._map) {
    for (Tile t : row) {
      os << (t == Tile::WALL ? '#' : '.');
    }
    os << '\n';
  }
  return os;
}

const BspNode *MapGenerator::findLeafContaining(const BspNode *node,
                                                vec2f p) const {
  if (!node || !node->rect().contains(p.x, p.y)) {
    return nullptr;
  }
  if (node->is_leaf()) {
    return node;
  } else {
    const BspNode *hit = findLeafContaining(node->left(), p);
    if (hit) {
      return hit;
    } else {
      return findLeafContaining(node->right(), p);
    }
  }
}

std::vector<std::vector<bool>> MapGenerator::floodfill(vec2i bfs_start) {
  std::vector<std::vector<bool>> msk(_map.size(),
                                     std::vector<bool>(_map[0].size(), false));
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

void MapGenerator::createRoom(const BspNode *node) {
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
    for (int y = 0; y < static_cast<int>(r.height); y++) {
      for (int x = 0; x < static_cast<int>(r.width); x++) {
        _map[r.y + y][r.x + x] = gen[y][x];
      }
    }
    _leafRooms.push_back(r);
  }

  createRoom(node->left());
  createRoom(node->right());
}

vec2i MapGenerator::connectSubtree(const BspNode *node) {
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
      for (int y = 0; y < static_cast<int>(r.height); y++) {
        for (int x = 0; x < static_cast<int>(r.width); x++) {
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

vec2i MapGenerator::findFloorCell(const Rect &r) {
  for (int y = 0; y < static_cast<int>(r.height); y++) {
    for (int x = 0; x < static_cast<int>(r.width); x++) {
      if (_map[r.y + y][r.x + x] == Tile::FLOOR) {
        return {r.x + x, r.y + y};
      }
    }
  }
  return {-1, -1};
}

void MapGenerator::carveCorridor(vec2i a, vec2i b) {
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

void MapGenerator::enforceBorder() {
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
