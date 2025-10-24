#include "../Headers/SceneHandler.h"


SceneHandler::SceneHandler() {}
SceneHandler::SceneHandler(std::wstring& map) // pass a map and construct a scene from that
{}


bool SceneHandler::IsOccupied(int x, int y) const {
    if( _objectContainer.contains(std::make_tuple(x, y)) ) {
        return true;
    }
}
