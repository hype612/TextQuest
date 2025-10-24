#include "../Headers/WindowsRenderer.h"

#if (defined (_WIN32) || defined (_WIN64))
void WindowsRenderer::Init() {
	_Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	_bytesWritten = 0;
}


void WindowsRenderer::OverwriteBuffer(wchar_t* newBuffer) {
    _screenBuffer = newBuffer;
}
void WindowsRenderer::PrintBuffer() {
    WriteConsoleOutputCharacter(_console, _screenBuffer, _screenWidth * _screenHeight, { 0,0 }, &dwBytesWritten);
}


std::tuple<int, int> WindowsRenderer::GetScreenSize() {
    return std::make_tuple(_screenWidth, _screenHeight);
}

void WindowsRenderer::SetScreenSize(int x, int y) {
    _screenWidth  = x;
    _screenHeight = y;
    delete _screenBuffer;
    _screenBuffer = new wchar_t[_screenWidth * _screenHeight];
}

WindowsRenderer::~WindowsRenderer() {
    delete _screenBuffer;
}
#endif // OS Check end
