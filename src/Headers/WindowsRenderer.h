#ifndef WINDOWSRENDERER_H
#define WINDOWSRENDERER_H

#if (defined (_WIN32) || defined (_WIN64))


#include "./IRenderer.h"
#include <Windows.h>

class WindowsRenderer : public IRenderer {
public:
    void Init() override;
    void OverwriteBuffer(wchar_t* newBuffer) override;
    void PrintBuffer() override;

    int GetScreenSize() override;
    void SetScreenSize(int x, int y) override;

    ~WindowsRenderer();
private:
    HANDLE _console;
    DWORD _bytesWritten;
}

#endif // OS check end
#endif // WINDOWSRENDERER_H
