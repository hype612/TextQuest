#ifndef ENEMYBEHAVIORCONTROLLER_H
#define ENEMYBEHAVIORCONTROLLER_H

#include "../Headers/EntityAction.h"
#include "IBehaviorController.h"
#include "ICollidable.h"
#include "IEnemyStateObserver.h"
#include <string>
#include <vector>

class EnemyBehaviorController : public IBehaviorController {
public:
  EnemyBehaviorController(std::vector<vec2f> patrol_points,
                           float shootCooldown, float shootDistance,
                           float reactionDelay = 0.f, float missChance = 0.f);
  vec2f Tick(Entity &self, float delta) override;
  void onCollision(Entity &self, ICollidable &other) override;
  virtual void onVisible(Entity &self, Entity &other) override;
  virtual void onHit(Entity &self, Entity &other) override;

  // Non-owning; the observer must outlive this controller (or be reset to
  // nullptr first).
  void setStateObserver(IEnemyStateObserver *observer) {
    _stateObserver = observer;
  }

private:
  void patrol(Entity &self, vec2f &acc_intent, float delta);
  void chase(Entity &self, vec2f &acc_intent, float delta);
  void attack(Entity &self, float delta);
  void shoot(Entity &, float delta);

  enum class State { IDLE, CHASE, ATTACK };
  /*
   * For Enemy, the valid state-changes are:
   * IDLE -> CHASE
   * IDLE -> ATTACK
   * CHASE -> ATTACK
   * ATTACK -> CHASE
   * NOTE: Enemy cannot go back to idle once he
   * sees the player
   */
  void updateState(Entity &self);
  vec2f moveIntent(EntityAction dir, Entity &self, float delta);
  std::string stateName(State s) const;

  State _state;
  std::vector<vec2f> _patrolPoints;
  Entity *_targetE;
  int _patrolIdx = 0;
  int _patrolDir = 1;
  const float _patrolArriveDistSq = 0.04f;
  const float _patrolFaceTolerance = 0.3f;
  float _lastShot = 0.f;
  const float _shootCooldown;
  const float _maxShootDist;
  // Time between first noticing the player and the first shot.
  const float _reactionDelay;
  const float _missChance; // 0..1
  IEnemyStateObserver *_stateObserver = nullptr;
};

#endif // ENEMYBEHAVIORCONTROLLER_H
