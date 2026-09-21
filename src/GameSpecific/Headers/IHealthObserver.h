#ifndef IHEALTHOBSERVER_H
#define IHEALTHOBSERVER_H

class IHealthObserver {
public:
  virtual ~IHealthObserver() = default;
  virtual void onHealthChanged(int current) = 0;
};

#endif // IHEALTHOBSERVER_H
