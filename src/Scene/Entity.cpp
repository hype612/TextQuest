#include "../Headers/Entity.h"


// =============
// Constructors
// =============

Entity::Entity() : _id(-1), _x(-1), _y(-1), _mapSign(L'E'), _texture(nullptr), _health(-1), _state(EntityState::IDLE)
{
    texture_mask = std::vector(1, 0);
}


Entity::Entity(int id, int x, int y, wchar_t mapSign, std::wstring* tex, int initHP, EntityState initState) : _id(id), _x(x), _y(y), _mapSign(mapSign), _texture(tex), _health(initHP), _state(initState)
{
    texture_mask = TextureMapper::GenerateTextureMask(*_texture);
}

// =============
// Getters	
// =============
int Entity::X()
{
	return _x;
}
int Entity::Y()
{
	return _y;
}

std::tuple<int, int> Entity::getCoordinates()
{
	return std::tuple<int, int>(_x, _y);
}

std::wstring* Entity::getTexturePointer()
{
    return _texture;
}

std::wstring Entity::getTexture()
{
    return *_texture;
}

int Entity::getHP()
{
	return _health;
}



// =============
// Setters
// =============


void Entity::setX(int x)
{
	_x = x;
}
void Entity::setY(int y)
{
	_y = y;
}

void Entity::modifyHealth(int amount)
{
	_health += amount;
	if (_health <= 0)
	{
		// destroy object
	}
}
