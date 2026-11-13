#ifndef NOTCURSESCURSESINPUTHANDLER_H_
#define NOTCURSESCURSESINPUTHANDLER_H_

#if (defined(LINUX) || defined(__linux__))
#include "./EntityAction.h"
#include "./IInputHandler.h"
#include <array>
#include <cstdint>
#include <memory>
#include <notcurses/notcurses.h>
#include <unordered_set>

class NotcursesInputHandler : public IInputHandler {
public:
  NotcursesInputHandler(std::shared_ptr<notcurses> nc);
  void Init() override;
  void ReceiveInput() override;
  bool keyDown(EntityAction dir) const override;
  bool rawKeyPressed(uint32_t key) const override;
  void resetInputState() override;
  ~NotcursesInputHandler() = default;

private:
  void KeyEvent(char in[], ncintype_e evtype);

  std::shared_ptr<notcurses> _nc;
  std::array<bool, entityActionCount> _mvmtKeyStates;
  std::unordered_set<char32_t> _pressedThisFrame;
};

#endif // OS Check end
#endif // NOTCURSESCURSESINPUTHANDLER_H_
