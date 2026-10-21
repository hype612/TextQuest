#ifndef ENEMYARCHETYPE_H
#define ENEMYARCHETYPE_H

#include <string>
#include <vector>

enum class EnemyKind { FERAL_GHOUL };

// Difficulty-independent base stats for one enemy type. Everything the Entity
// ctor needs except the spawn-specific parts (Transform, behavior controller).
// Difficulty scaling is applied when an enemy is spawned, never stored here.
struct EnemyArchetype {
  // Loaded once per kind, lv0 (brightest) first.
  std::vector<std::string> textures;
  int health;
  float collisionRadius;
  float fovDegrees;
  float viewDistance;
  float moveSpeed;
  float turnSpeed;
  float damage;
  float attackCooldown;
};

#endif // !ENEMYARCHETYPE_H
