#ifndef IENEMYSTATEOBSERVER_H
#define IENEMYSTATEOBSERVER_H

#include <string>

class IEnemyStateObserver {
public:
  virtual ~IEnemyStateObserver() = default;
  virtual void onStateChanged(const std::string &state) = 0;
};

#endif // IENEMYSTATEOBSERVER_H
