#include "../Headers/TerrainCollidable.h"
#include "../Headers/ICollidable.h"
#include "../Headers/Vec2i.h"

TerrainCollidable::TerrainCollidable(vec2i map_pos) : _position(map_pos) {}
Collidable TerrainCollidable::collidableKind() const {
  return Collidable::WALL;
}
const vec2i &TerrainCollidable::position() const { return _position; }
