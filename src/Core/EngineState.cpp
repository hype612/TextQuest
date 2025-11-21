#include "../Headers/EngineState.h"

EngineState *EngineState::instancePtr = nullptr;
int EngineState::screenHeight = -1;
int EngineState::screenWidth = -1;
RenderAssetManager *EngineState::globalRenderAssetManager = nullptr;
SceneManager *EngineState::globalSceneManager = nullptr;
bool EngineState::gameRunningf = false;
