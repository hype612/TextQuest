#include "../Headers/Entity.h"


// =============
// Constructors
// =============

Entity::Entity() : _id(-1), _x(-1), _y(-1), _mapSign(L'E'), _texture(nullptr), _health(-1), _state(EntityState::IDLE)
{
    texture_mask = std::vector(1, 0);
}


Entity::Entity(int x, int y, std::wstring* tex, std:: wstring* texMask, int initHP, EntityState initState) 
  : _id(-1), _x(x), _y(y), _texMapper(tex), _health(initHP), _state(initState)
{}

// =============
// Getters	
// =============
float Entity::X()
{
	return _x;
}
float Entity::Y()
{
	return _y;
}


int Entity::discreteX() 
{
  return (int)_x;
}
int Entity::discreteY() 
{
  return (int)_y;
}


std::tuple<int, int> Entity::getCoordinates()
{
	return std::tuple<int, int>(_x, _y);
}

std::wstring Entity::getTexture()
{
    return _texMapper.getTexture();
}

std::wstring Entity::getNextTexColumn(int height)
{
  return _texMapper.getNextColumn();
}

std::wstring Entity::getTexMask()
{
  return _maskMapper.getMask();
}

std::wstring Entity::getMaskColumn(int height)
{
  return _maskMapper.getMaskColumn();
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

void Entity::setID(int id)
{
  if (_id == -1)
   _id = id;
  return;
}


void Entity::modifyHealth(int amount)
{
	_health += amount;
	if (_health <= 0)
	{
		// destroy object
	}
}

void Entity::rescaleTexture(float distance)
{
  _texMapper.rescaleTexture(float distance);
}


