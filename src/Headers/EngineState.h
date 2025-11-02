#ifndef ENGINESTATE_H_
#define ENGINESTATE_H_


#include "MapManager.h"
#include "SceneManager.h"
#include "EntityManager.h"

enum class RunningOS { WINDOWS,
                       LINUX };

class EngineState {
public:
    EngineState(const EngineState& obj) = delete;

    static EngineState* GetInstance() {
        if (instancePtr == nullptr) {
            instancePtr = new EngineState();
        }
        return instancePtr;
    }

#if ( defined(LINUX) || defined(__linux__) )
    RunningOS opSystem = RunningOS::LINUX;
#endif
#if ( defined(_WIN32) || defined(_WIN64) )
    RunningOS opSystem = RunningOS::WINDOWS;
#endif
    int screenHeight = -1;
    int screenWidth  = -1;
    
    static SceneManager*  _globalSceneManager;
    static EntityManager* _globalEntityManager;
    static MapManager*    _globalMapManager;
private:
    EngineState() {}
    
    


    static EngineState* instancePtr;
    
};

#endif // ENGINESTATEVARS_H_
