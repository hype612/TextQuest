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
  Entity(int x, int y, std::string *tex, int initHP, EntityState initState);

  virtual void process();

  // getters
  int ID() const;
  float X() const;
  float Y() const;
  int discreteX() const;
  int discreteY() const;

  std::tuple<int, int> getCoordinates();
  std::string getTexture() const;
  // std::string getNextTexColumn(int height);
  std::string getTexColumnAt(int height, float hitpoint);
  std::string getTexColumnAt(int height, float hitpoint, float distance);
  std::vector<int> getMaskColumnAt(int height, float hitpoint);
  const std::vector<int> &getTexMask() const;
  // std::vector<int> getMaskColumn(int height) const;

  int getHP();

  // setters
  void setX(int x);
  void setY(int y);
  void setID(int id);
  void modifyHealth(int amount);

private:
  int _id;
  float _x;
  float _y;
  TextureMapper _texMapper;
  int _health;
  EntityState _state;
};

#endif // ENTITY_H
