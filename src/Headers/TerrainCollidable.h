#ifndef TERRAINCOLLIDABLE_H
#define TERRAINCOLLIDABLE_H

#include "ICollidable.h"
#include "Vec2i.h"

class TerrainCollidable : public ICollidable {
public:
  TerrainCollidable(vec2i map_pos);
  Collidable collidableKind() const override;
  const vec2i &position() const;

private:
  vec2i _position;
};

#endif // TERRAINCOLLIDABLE_H
