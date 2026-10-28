#ifndef ISHOOTOBSERVER_H
#define ISHOOTOBSERVER_H

class IShootObserver {
public:
  virtual ~IShootObserver() = default;
  virtual void onShoot() = 0;
};

#endif // !ISHOOTOBSERVER_H
