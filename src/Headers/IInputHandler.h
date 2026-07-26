#ifndef IINPUTHANDLER_H
#define IINPUTHANDLER_H

#include "MoveDirection.h"

class IInputHandler {
public:
  virtual void Init() = 0;
  virtual void ReceiveInput() = 0;
  virtual bool keyDown(MoveDirection dir) const = 0;
  virtual ~IInputHandler() = default;

protected:
  IInputHandler() {}
};

#endif // IINPUTHANDLER_H
