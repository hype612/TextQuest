#include "../Headers/NotcursesRenderer.h"
#include <notcurses/notcurses.h>

NotcursesRenderer::NotcursesRenderer(notcurses *nc) {}
void NotcursesRenderer::Init() {}
void NotcursesRenderer::OverwriteBuffer(char *newBuffer) {}
void NotcursesRenderer::PrintBuffer() {}
void NotcursesRenderer::PrintDebugInfo(const Player &player, float delta) {}
std::tuple<int, int> NotcursesRenderer::GetScreenSize() {}
NotcursesRenderer::~NotcursesRenderer() {}
