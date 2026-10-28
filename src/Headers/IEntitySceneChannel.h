#ifndef IENTITYSCENECHANNEL_H
#define IENTITYSCENECHANNEL_H

#include "Vec2f.h"

class IEntitySceneChannel {
public:
  virtual bool canMoveTo(const vec2f &dest, float radius) const = 0;
  virtual void playerDiedNotify() = 0;
};

#endif // IENTITYSCENECHANNEL_H
