#ifndef NOTCURSESCURSESINPUTHANDLER_H_
#define NOTCURSESCURSESINPUTHANDLER_H_

#if (defined(LINUX) || defined(__linux__))
#include "./IInputHandler.h"
#include "./MoveDirection.h"
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
  bool keyDown(MoveDirection dir) const override;
  bool rawKeyPressed(uint32_t key) const override;
  ~NotcursesInputHandler() = default;

private:
  void KeyEvent(char in[], ncintype_e evtype);

  std::shared_ptr<notcurses> _nc;
  std::array<bool, moveDirectionCount + 1> _mvmtKeyStates;
  std::unordered_set<char32_t> _pressedThisFrame;
};

#endif // OS Check end
#endif // NOTCURSESCURSESINPUTHANDLER_H_
