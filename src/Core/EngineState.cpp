#include "../Headers/EngineState.h"

EngineState *EngineState::GetInstance() {
  if (instancePtr == nullptr) {
    instancePtr = new EngineState();
  }
  return instancePtr;
}

EngineState *EngineState::instancePtr = nullptr;
int EngineState::screenHeight = -1;
int EngineState::screenWidth = -1;
RenderAssetManager *EngineState::globalRenderAssetManager = nullptr;
SceneManager *EngineState::globalSceneManager = nullptr;
bool EngineState::gameRunningf = false;
