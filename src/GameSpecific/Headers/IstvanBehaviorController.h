#ifndef ISTVANBEHAVIORCONTROLLER_H
#define ISTVANBEHAVIORCONTROLLER_H

#include "../Headers/MoveDirection.h"
#include "IBehaviorController.h"
#include "ICollidable.h"
#include "IIstvanStateObserver.h"
#include <string>
#include <vector>

class IstvanBehaviorController : public IBehaviorController {
public:
  IstvanBehaviorController(std::vector<vec2f> patrol_points);
  vec2f Tick(Entity &self, float delta) override;
  void onCollision(Entity &self, ICollidable &other) override;
  virtual void onVisible(Entity &self, Entity &other) override;
  virtual void onHit(Entity &self, Entity &other) override;

  // Non-owning; the observer must outlive this controller (or be reset to
  // nullptr first).
  void setStateObserver(IIstvanStateObserver *observer) {
    _stateObserver = observer;
  }

private:
  void patrol(Entity &self, vec2f &acc_intent, float delta);
  void chase(Entity &self, vec2f &acc_intent, float delta);
  void attack(Entity &self, float delta);
  void shoot(Entity &, float delta);

  enum class State { IDLE, CHASE, ATTACK };
  /*
   * For Istvan, the valid state-changes are:
   * IDLE -> CHASE
   * IDLE -> ATTACK
   * CHASE -> ATTACK
   * ATTACK -> CHASE
   * NOTE: Istvan cannot go back to idle once he
   * sees the player
   */
  void updateState(Entity &self);
  vec2f moveIntent(MoveDirection dir, Entity &self, float delta);
  std::string stateName(State s) const;

  State _state;
  std::vector<vec2f> _patrolPoints;
  Entity *_targetE;
  int _patrolIdx = 0;
  int _patrolDir = 1;
  const float _patrolArriveDistSq = 0.04f;
  const float _patrolFaceTolerance = 0.3f;
  float _lastShot = 0.f;
  const float _shootCooldown = 1.f;
  const float _maxShootDist = 3.f;
  const int _misschance = 0; // for testing
  IIstvanStateObserver *_stateObserver = nullptr;
};

#endif // ISTVANBEHAVIORCONTROLLER_H
