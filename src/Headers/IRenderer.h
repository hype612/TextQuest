#ifndef IRENDERER_H
#define IRENDERER_H

#include "EngineState.h"
#include <string>
#include <tuple>
#include <vector>

class IRenderer {
public:
  virtual void Init() = 0;
  virtual void OverwriteBuffer(char *newBuffer) = 0;
  virtual void PrintBuffer() const = 0;

  virtual std::tuple<int, int> GetScreenSize() const = 0;
  // virtual void SetScreenSize(int x, int y) = 0; BASICALLY DEPRECATED
  virtual void PrintDebugInfo(const std::vector<std::string> &nfo) = 0;
  virtual ~IRenderer() = default;

protected:
  IRenderer() {}
  int &_screenHeight = EngineState::GetInstance()->screenHeight;
  int &_screenWidth = EngineState::GetInstance()->screenWidth;
};

#endif // IRENDERER_H
