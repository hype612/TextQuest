#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <tuple>
#include <vector>

#include "TextureMapper.h"

enum EntityState { IDLE, WALKING, FIGHT };

class Entity {
public:
  // constructors
  Entity(int x, int y, std::wstring *tex, int initHP, EntityState initState);

  virtual void process();

  // getters
  int ID() const;
  float X() const;
  float Y() const;
  int discreteX() const;
  int discreteY() const;

  std::tuple<int, int> getCoordinates();
  std::wstring getTexture() const;
  std::wstring getNextTexColumn(int height);
  const std::vector<int> &getTexMask() const;
  std::vector<int> getMaskColumn(int height) const;

  int getHP();

  // setters
  void setX(int x);
  void setY(int y);
  void setID(int id);
  void modifyHealth(int amount);

  void rescaleTexture(float distance);

private:
  int _id;
  float _x;
  float _y;
  // std::wstring* _texture;
  // std::vector<int> texture_mask;
  TextureMapper _texMapper;
  int _health;
  EntityState _state;
};

#endif // ENTITY_H
