#ifndef ENEMYARCHETYPE_H
#define ENEMYARCHETYPE_H

#include <string>
#include <vector>

struct EnemyArchetype {
  // Loaded once per kind, lv0 (brightest) first.
  std::vector<std::string> textures;
  int health;
  int damage;
  float collisionRadius;
  float fovDegrees;
  float viewDistance;
  float moveSpeed;
  float turnSpeed;
  float attackCooldown;
};

#endif // !ENEMYARCHETYPE_H
