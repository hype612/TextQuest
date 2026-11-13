#ifndef TQ_TEST_SCRIPTEDINPUT_H
#define TQ_TEST_SCRIPTEDINPUT_H

#include "IInputHandler.h"
#include "EntityAction.h"
#include <array>
#include <set>

// IInputHandler whose key state is set directly by the test. Tests change the
// state between ticks themselves.
class ScriptedInput : public IInputHandler {
public:
  void Init() override {}
  void ReceiveInput() override {}
  void resetInputState() override {
    _down.fill(false);
    _raw.clear();
  }
  bool keyDown(EntityAction dir) const override {
    return _down[static_cast<size_t>(dir)];
  }
  bool rawKeyPressed(uint32_t key) const override { return _raw.contains(key); }

  void setKeyDown(EntityAction dir, bool down) {
    _down[static_cast<size_t>(dir)] = down;
  }
  void pressRaw(uint32_t key) { _raw.insert(key); }
  void releaseRaw(uint32_t key) { _raw.erase(key); }

private:
  std::array<bool, entityActionCount> _down{}; 
  std::set<uint32_t> _raw;
};

#endif // TQ_TEST_SCRIPTEDINPUT_H
