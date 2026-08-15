#ifndef ENTITY_H
#define ENTITY_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "IBehaviorController.h"
#include "ICollidable.h"
#include "MoveDirection.h"
#include "TextureMapper.h"
#include "Transform.h"
#include "Vec2f.h"

class Entity : public ICollidable {
public:
  // =============
  // Constructors
  // =============
  Entity(std::unique_ptr<IBehaviorController> behaviourCtrl, Transform pos,
         std::string *tex, int initHP, float collisionRadius, bool isPlayer,
         float fov_in_deg, float viewDistance);
  Entity(std::unique_ptr<IBehaviorController> behaviourCtrl, Transform pos,
         std::vector<std::string> texVec, int initHP, float collisionRadius,
         bool isPlayer, float fov_in_deg, float viewDistance);
  Entity() = delete;

  // =============
  // Process
  // =============

  vec2f process(float delta);
  void onCollision(ICollidable &other);
  void onVisible(Entity &other);
  // =============
  // Getters
  // =============

  Collidable collidableKind() const override;
  int ID() const;
  bool isPlayer() const;
  float viewDistance() const;
  float fov() const;
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

  int health() const;
  float moveSpeedOnDir(MoveDirection direction) const;
  float collisionRadius() const;

  // =============
  // Setters
  // =============

  void move(float delta, MoveDirection dir);
  void setID(int id);
  void setHealth(int new_value);
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
  float _collisionRadius;
  bool _isPlayer;
  float _fov;
  float _viewDistance;

  std::unique_ptr<IBehaviorController> _behaviourCtrl;
  std::array<float, moveDirectionCount> _directionalSpeeds;
};

#endif // ENTITY_H
