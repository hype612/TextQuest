#ifndef ENEMYARCHETYPE_H
#define ENEMYARCHETYPE_H

#include <string>
#include <vector>

struct EnemyArchetype {
  // Loaded once per kind, lv0 (brightest) first.
  std::vector<std::string> textures;
  int health;
  float damage;
  // movement footprint (walls, spacing)
  float collisionRadius;
  // size as a hitscan target
  float hitRadius;
  float fovDegrees;
  float viewDistance;
  float moveSpeed;
  float turnSpeed;
  float shootCooldown;
  // Time between first noticing the player and the first shot.
  float reactionDelay;
  // Chance (0..1) that a shot is not fired at all.
  float missChance;
};

#endif // !ENEMYARCHETYPE_H
