#ifndef TQ_TEST_RECORDINGOBSERVERS_H
#define TQ_TEST_RECORDINGOBSERVERS_H

#include "GameSpecific/Headers/IHealthObserver.h"
#include "GameSpecific/Headers/IEnemyStateObserver.h"
#include "GameSpecific/Headers/IShootObserver.h"
#include <string>
#include <vector>

struct RecordingHealthObserver : IHealthObserver {
  std::vector<int> values;
  void onHealthChanged(int current) override { values.push_back(current); }
};

struct RecordingShootObserver : IShootObserver {
  int shots = 0;
  void onShoot() override { ++shots; }
};

struct RecordingEnemyStateObserver : IEnemyStateObserver {
  std::vector<std::string> states;
  void onStateChanged(const std::string &state) override {
    states.push_back(state);
  }
};

#endif // TQ_TEST_RECORDINGOBSERVERS_H
