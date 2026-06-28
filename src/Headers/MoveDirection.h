#ifndef MOVEDIRECTION_H
#define MOVEDIRECTION_H
#include <cstddef>

constexpr size_t moveDirectionCount = 6;

enum class MoveDirection {
  FORWARD,
  BACKWARD,
  STRAFE_LEFT,
  STRAFE_RIGHT,
  TURN_LEFT,
  TURN_RIGHT
};

#endif // !MOVEDIRECTION
