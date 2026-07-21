#ifndef IINPUTHANDLER_H
#define IINPUTHANDLER_H

#include "./player.h"
#include <cmath>

class IInputHandler {
public:
  virtual void Init() = 0;
  virtual void ReceiveMovementInput(float delta) = 0;
  virtual ~IInputHandler() = default;

protected:
  IInputHandler(Player &player) : _player(player) {}
  Player &_player;
};

#endif // IINPUTHANDLER_H
