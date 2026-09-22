#ifndef IISTVANSTATEOBSERVER_H
#define IISTVANSTATEOBSERVER_H

#include <string>

class IIstvanStateObserver {
public:
  virtual ~IIstvanStateObserver() = default;
  virtual void onStateChanged(const std::string &state) = 0;
};

#endif // IISTVANSTATEOBSERVER_H
