#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "../../Headers/Vec2f.h"
#include "../../Headers/Vec2i.h"
#include "bsp_node.hpp"
#include "cellularAutomata.hpp"
#include <iosfwd>
#include <vector>

constexpr int ideal_generation_num = 4;

enum class SpawnType { PLAYER, ENEMY, BOSS };
struct SpawnPoint {
  SpawnType type;
  vec2i pos;
};

class MapGenerator {
public:
  MapGenerator(const Rect &parent_area, const part_params &params,
               int min_wall_neighbor_count, float noise_distribution);

  void generateSpawnPoints(float max_enemy_density, int boss_room_count);
  std::vector<vec2f> generatePatrolPoints(vec2f spawn_point,
                                          unsigned int count) const;

  const std::vector<SpawnPoint> &spawnPoints() const { return _spawnPoints; }
  const std::vector<std::vector<Tile>> &map() { return _map; }

  // TODO: remove if testing is done
  friend std::ostream &operator<<(std::ostream &os, const MapGenerator &mg);

  // TODO: remove if testing is done
  friend std::ostream &printWithSpawns(std::ostream &os,
                                       const MapGenerator &mg);

private:
  // fill the rects created by BSP
  void FillRooms();

  // create walkable links between rooms, then fill in any floor
  // that can't be reached from them (isolated cave pockets)
  void createLinks();

  std::vector<vec2i> accessibleFloorCells(const Rect &r) const;
  const BspNode *findLeafContaining(const BspNode *node, vec2f p) const;

  std::vector<std::vector<bool>> floodfill(vec2i bfs_start);
  void createRoom(const BspNode *node);

  // returns fully-connected subtree, for the parent to link into.
  vec2i connectSubtree(const BspNode *node);
  vec2i findFloorCell(const Rect &r);

  // Bulldozes an L-shaped, one-tile-wide path between two floor cells:
  // horizontal run at a's row, then vertical run at b's column.
  void carveCorridor(vec2i a, vec2i b);
  void enforceBorder();

  BspNode _bspRoot;
  int _ca_min_wall_neighbor_count;
  float _ca_noise_distribution;
  // x and y param are pretty much meaningless here
  // only keeping it like this for consistency
  Rect _parentArea;
  std::vector<std::vector<Tile>> _map;
  std::vector<std::vector<bool>> _accessibleMask;
  std::vector<Rect> _leafRooms;
  std::vector<SpawnPoint> _spawnPoints;
};

#endif // !MAPGENERATOR_H
