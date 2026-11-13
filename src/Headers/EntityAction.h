#ifndef ENTITYACTION_H
#define ENTITYACTION_H
#include <cstddef>

enum class EntityAction {
  FORWARD,
  BACKWARD,
  STRAFE_LEFT,
  STRAFE_RIGHT,
  TURN_LEFT,
  TURN_RIGHT,
  SHOOT,
  COUNT // sentinel, not a real action
};

constexpr size_t entityActionCount = static_cast<size_t>(EntityAction::COUNT);

#endif // !ENTITYACTION_H
