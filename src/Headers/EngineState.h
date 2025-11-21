#ifndef ENGINESTATE_H_
#define ENGINESTATE_H_

class SceneManager;
class RenderAssetManager;

enum class RunningOS { WINDOWS, LINUX };

class EngineState {
public:
  EngineState(const EngineState &obj) = delete;

  static EngineState *GetInstance() {
    if (instancePtr == nullptr) {
      instancePtr = new EngineState();
    }
    return instancePtr;
  }

#if (defined(LINUX) || defined(__linux__))
  RunningOS opSystem = RunningOS::LINUX;
#endif
#if (defined(_WIN32) || defined(_WIN64))
  RunningOS opSystem = RunningOS::WINDOWS;
#endif
  static int screenHeight;
  static int screenWidth;

  static RenderAssetManager *globalRenderAssetManager;
  static SceneManager *globalSceneManager;
  static bool gameRunningf;

private:
  EngineState() {}

  static EngineState *instancePtr;
};

#endif // ENGINESTATEVARS_H_
