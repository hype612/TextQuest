#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <tuple>
#include <vector>

#include "TextureMapper.h"
#include "Transform.h"
#include "Vec2f.h"

enum EntityState { IDLE, WALKING, FIGHT };

class Entity {
public:
  // =============
  // Constructors
  // =============

  Entity(float x, float y, float angle, std::string *tex, int initHP,
         EntityState initState);
  Entity(Transform pos, std::string *tex, int initHP, EntityState initState);

  // =============
  // Process
  // =============

  void process();

  // =============
  // Getters
  // =============

  int ID() const;
  float X() const;
  float Y() const;
  float Angle() const;

  vec2f coordinates();
  const Transform &transform() const;

  std::string getTexture() const;
  std::string getTexColumnAt(int height, float hitpoint, int wallTop) const;
  std::string getTexColumnAt(int height, float hitpoint, int wallTop,
                             int shadingIdx) const;
  std::vector<int> getMaskColumnAt(int height, float hitpoint);
  const std::vector<int> &getTexMask() const;

  int getHP();

  // =============
  // Setters
  // =============

  void setX(int x);
  void setY(int y);
  void setID(int id);
  void modifyHealth(int amount);

private:
  int _id;
  Transform _transform;
  TextureMapper _texMapper;
  int _health;
  EntityState _state;
};

#endif // ENTITY_H
