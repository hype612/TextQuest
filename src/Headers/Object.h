#ifndef OBJECT_H
#define OBJECT_H

#include <tuple>
#include <string>
#include "../Headers/TextureMapper.h"

class Object
{
public:
    Object(int x, int y, std::wstring texture);

    
    // getters
    int X();
    int Y();
    std::tuple<int, int> Pos();
    std::wstring Texture();

    // setters
    void setX(int newX);
    void setY(int newY);
    void setPos(std::tuple<int, int> newPos);
    // moving/sliding motions are not necessary for lifeless objects


    void UpdateTexture(float distance);

private:
    int _x;
    int _y;
    std::tuple<int, int> _pos;
    const std::wstring _initTexture;
    std::wstring _texture;
    TextureMapper _tMapper;
};


#endif // OBJECT_H