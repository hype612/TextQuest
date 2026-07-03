#ifndef IRENDERER_H
#define IRENDERER_H

#include "EngineState.h"
#include "player.h"
#include <string>
#include <tuple>

class IRenderer {
public:
  virtual void Init() = 0;
  virtual void OverwriteBuffer(char *newBuffer) = 0;
  virtual void PrintBuffer() = 0;

  virtual std::tuple<int, int> GetScreenSize() = 0;
  virtual void SetScreenSize(int x, int y) = 0;
  virtual void PrintDebugInfo(const Player &p, float delta) = 0;
  virtual ~IRenderer() = default;

protected:
  IRenderer() {}
  int &_screenHeight = EngineState::GetInstance()->screenHeight;
  int &_screenWidth = EngineState::GetInstance()->screenWidth;
};

#endif // IRENDERER_H
