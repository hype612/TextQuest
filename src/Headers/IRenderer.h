#ifndef IRENDERER_H
#define IRENDERER_H

#include <string>
#include <tuple>
#include "EngineState.h"

class IRenderer {
public:
    virtual void Init() = 0;
    virtual void OverwriteBuffer(wchar_t* newBuffer) = 0;
    virtual void PrintBuffer() = 0;

    virtual std::tuple<int, int> GetScreenSize() = 0;
    virtual void SetScreenSize(int x, int y) = 0;
protected:
    IRenderer() {}
    wchar_t* _screenBuffer;
    int& _screenHeight = EngineState::GetInstance()->screenHeight;
    int& _screenWidth  = EngineState::GetInstance()->screenWidth;
};



#endif // IRENDERER_H
