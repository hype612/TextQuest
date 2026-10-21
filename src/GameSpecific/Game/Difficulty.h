#ifndef DIFFICULTY_H
#define DIFFICULTY_H

struct Difficulty {
  float enemyDamage;
  float enemyHealth;
  float enemySpeed;
  float attackCooldown;
  float enemyDensity;
};

inline constexpr Difficulty EASY{.5f, .75f, .8f, 1.5f, 0.001f};
inline constexpr Difficulty NORMAL{1.f, 1.f, 1.f, 1.f, 0.02f};
inline constexpr Difficulty HARD{1.5f, 1.5f, 1.2f, 0.8f, 0.05f};

#endif // !DIFFICULTY_H
