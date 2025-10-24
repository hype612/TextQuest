#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <tuple>
#include <vector>

#include "TextureMapper.h" 

enum EntityState
{
	IDLE, WALKING, FIGHT 
}; 


class Entity
{
private:
    int _id;
	int _x;
	int _y;
	wchar_t _mapSign;
	std::wstring* _texture;
	std::vector<int> texture_mask;
	int _health;
	EntityState _state;
public:
	// constructors
	Entity();
	Entity(int id, int x, int y, wchar_t mapSign, std::wstring* tex, int initHP, EntityState initState);
	
	// getters
	int ID();	
	int X();
	int Y();

	std::tuple<int, int> getCoordinates();
	std::wstring* getTexturePointer();
	std::wstring getTexture();


	int getHP();

	// setters
	void setX(int x);
	void setY(int y);

	void modifyHealth(int amount);
};


#endif // ENTITY_H
