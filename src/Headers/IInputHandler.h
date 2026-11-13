#ifndef IINPUTHANDLER_H
#define IINPUTHANDLER_H

#include "EntityAction.h"
#include <cstdint>

class IInputHandler {
public:
  virtual void Init() = 0;
  virtual void ReceiveInput() = 0;
  virtual bool keyDown(EntityAction dir) const = 0;
  virtual void resetInputState() = 0;

  // fallback way to query raw key states w/o
  // routing through enums
  virtual bool rawKeyPressed(uint32_t key) const = 0;
  virtual ~IInputHandler() = default;

protected:
  IInputHandler() {}
};

#endif // IINPUTHANDLER_H
