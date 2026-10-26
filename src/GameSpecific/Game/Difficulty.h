#ifndef DIFFICULTY_H
#define DIFFICULTY_H

// NOTE: keep in mind that all of these
// ARE scalers not exact values.
struct Difficulty {
  float enemyDamage;
  float enemyHealth;
  float enemySpeed;
  float attackCooldown;
  float fovDegrees;
  float viewDistance;
  float enemyDensity;
  float reactionDelay;
  float missChance;
};

//                              dmg   hp    speed cooldown fov   view  density
//                              reaction miss
inline constexpr Difficulty EASY{.5f,  .75f, .8f,   1.5f,  .8f,   .75f,
                                 0.003f, 1.5f, 1.5f};
inline constexpr Difficulty NORMAL{1.f,  1.f, 1.f,    1.f, 1.f,
                                   1.f, 0.006f, 1.f, 1.f};
inline constexpr Difficulty HARD{1.5f,  1.5f, 1.2f,   .8f,  1.1f,
                                 1.25f, 0.012f, .7f,  .5f};

#endif // !DIFFICULTY_H
