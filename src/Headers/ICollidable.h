#ifndef ICOLLIDABLE_H
#define ICOLLIDABLE_H

enum class Collidable { ENTITY, WALL };

class ICollidable {
public:
  virtual ~ICollidable() = default;
  virtual Collidable collidableKind() const = 0;
};

#endif // ICOLLIDABLE_H
