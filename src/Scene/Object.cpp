#include "../Headers/Object.h"

Object::Object(int x, int y, std::wstring texture)
    : _x(x), _y(x), _pos(x, y), _initTexture(texture), _tMapper(texture) {}


// getters
int Object::X() { return _x; }
int Object::Y() { return _y; }
std::tuple<int, int> Object::Pos() { return _pos; }
std::wstring Object::Texture() { return _texture; }


// setters
void Object::setX(int newX) { _x = newX; }
void Object::setY(int newY) { _y = newY; }
void Object::setPos(std::tuple<int, int> newPos) { _pos = newPos; }


// rescales the texture to the desired height and width
void Object::UpdateTexture(float distance)
{
    
}
