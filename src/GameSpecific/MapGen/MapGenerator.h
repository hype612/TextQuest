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

  // fill the rects created by BSP
  void FillRooms();

  // create walkable links between rooms, then fill in any floor
  // that can't be reached from them (isolated cave pockets)
  void createLinks();

  std::vector<vec2f> generatePatrolPoints(vec2f spawn_point,
                                          unsigned int count) const;

  // Must run after createLinks(). Picks one player room, up to
  // boss_room_count boss rooms, and rolls every remaining room's floor
  // cells against max_enemy_density.
  void generateSpawnPoints(float max_enemy_density, int boss_room_count);
  const std::vector<SpawnPoint> &spawnPoints() const { return _spawnPoints; }

  // TODO: remove if testing is done
  friend std::ostream &operator<<(std::ostream &os, const MapGenerator &mg);

  // Overlays spawn markers (P/B/E) on top of the plain wall/floor rendering
  // from operator<<. Defined externally (src/GameSpecific/MapGen/main.cpp),
  // hence the friend declaration rather than a member.
  // TODO: remove if testing is done
  friend std::ostream &printWithSpawns(std::ostream &os,
                                       const MapGenerator &mg);

private:
  std::vector<vec2i> accessibleFloorCells(const Rect &r) const;
  const BspNode *findLeafContaining(const BspNode *node, vec2f p) const;

  std::vector<std::vector<bool>> floodfill(vec2i bfs_start);
  void createRoom(const BspNode *node);

  // Post-order: connect both children first, then link this node's two
  // subtrees together. Returns a floor-cell anchor point somewhere in the
  // now-fully-connected subtree, for the parent to link into.
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
