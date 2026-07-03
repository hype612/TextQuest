#ifndef NOTCURSESCURSESINPUTHANDLER_H_
#define NOTCURSESCURSESINPUTHANDLER_H_

#if (defined(LINUX) || defined(__linux__))
#include "./IInputHandler.h"
#include "./MoveDirection.h"
#include "./player.h"
#include <memory>
#include <notcurses/notcurses.h>
#include <unordered_set>

class NotcursesInputHandler : public IInputHandler {
public:
  NotcursesInputHandler(Player &player, std::shared_ptr<notcurses> nc);
  void Init() override;
  void ReceiveMovementInput(float delta) override;
  ~NotcursesInputHandler() = default;

private:
  void KeyEvent(char in[], ncintype_e evtype);

  std::shared_ptr<notcurses> _nc;
  std::array<bool, moveDirectionCount> _mvmtKeyStates;
};

#endif // OS Check end
#endif // NOTCURSESCURSESINPUTHANDLER_H_
