#ifndef ENTITY_H
#define ENTITY_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "IBehaviorController.h"
#include "MoveDirection.h"
#include "TextureMapper.h"
#include "Transform.h"
#include "Vec2f.h"

enum EntityState { IDLE, WALKING, FIGHT };

class Entity {
public:
  // =============
  // Constructors
  // =============

  Entity(std::unique_ptr<IBehaviorController> behaviourCtrl, Transform pos,
         std::string *tex, int initHP);
  Entity() = delete;

  // =============
  // Process
  // =============

  vec2f process(float delta);

  // =============
  // Getters
  // =============

  int ID() const;

  const Transform &transform() const;

  std::string getTexture() const;
  std::string getTexColumnAt(int height, float hitpoint, int wallTop) const;
  std::string getTexColumnAt(int height, float hitpoint, int wallTop,
                             int shadingIdx) const;
  std::vector<int> getMaskColumnAt(int height, float hitpoint);
  const std::vector<int> &getTexMask() const;
  std::string scaledTex(unsigned int width, unsigned int height,
                        int shadingIdx) const;
  std::vector<int> scaledMask(unsigned int width, unsigned int height) const;

  int getHP();
  float moveSpeedOnDir(MoveDirection direction) const;

  // =============
  // Setters
  // =============

  void move(float delta, MoveDirection dir);
  void setID(int id);
  void modifyHealth(int amount);
  void setTransform(const Transform &new_transform);

  void addToX(float rval_x);
  void addToY(float rval_y);
  void addToAngle(float rval_a);
  void addToMoveSpeedOnDirection(float new_speed, MoveDirection direction);
  void addToMoveSpeedAllDirections(float new_speed);
  void addToTurnSpeedAlldirections(float new_speed);

  void setMoveSpeedOnDirection(float new_speed, MoveDirection direction);
  void setMoveSpeedAllDirectons(float new_speed);
  void setTurnSpeedAlldirections(float new_speed);

private:
  int _id;
  Transform _transform;
  TextureMapper _texMapper;
  int _health;
  EntityState _state;
  std::unique_ptr<IBehaviorController> _behaviourCtrl;
  std::array<float, moveDirectionCount> _directionalSpeeds;
};

#endif // ENTITY_H
