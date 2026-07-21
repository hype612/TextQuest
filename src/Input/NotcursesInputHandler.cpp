#include "../Headers/NotcursesInputHandler.h"
#include "./Logger.h"
#include <notcurses/notcurses.h>

NotcursesInputHandler::NotcursesInputHandler(Player &player,
                                             std::shared_ptr<notcurses> nc)
    : IInputHandler(player), _nc(nc) {
  Logger::setLogLevel(LogLevel::INFO);
  for (size_t i = 0; i < _mvmtKeyStates.size(); i++) {
    _mvmtKeyStates[i] = false;
  }
}

void NotcursesInputHandler::Init() {}
void NotcursesInputHandler::ReceiveMovementInput(float delta) {

  ncinput in_char;
  timespec ts{0, 0};
  uint32_t rc;
  while ((rc = notcurses_get(_nc.get(), &ts, &in_char)) > 0) {
    KeyEvent(in_char.utf8, in_char.evtype);
  }

  if (_mvmtKeyStates[static_cast<int>(MoveDirection::FORWARD)] == true)
    _player.move(delta, MoveDirection::FORWARD);
  if (_mvmtKeyStates[static_cast<int>(MoveDirection::BACKWARD)] == true)
    _player.move(delta, MoveDirection::BACKWARD);
  if (_mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_LEFT)] == true)
    _player.move(delta, MoveDirection::STRAFE_LEFT);
  if (_mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_RIGHT)] == true)
    _player.move(delta, MoveDirection::STRAFE_RIGHT);
  if (_mvmtKeyStates[static_cast<int>(MoveDirection::TURN_LEFT)] == true)
    _player.move(delta, MoveDirection::TURN_LEFT);
  if (_mvmtKeyStates[static_cast<int>(MoveDirection::TURN_RIGHT)] == true)
    _player.move(delta, MoveDirection::TURN_RIGHT);
}

void NotcursesInputHandler::KeyEvent(char in[], ncintype_e evtype) {
  switch (*in) {
  case 'w':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::FORWARD)] = true;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::FORWARD)] = false;
    }
    break;
  case 'a':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_LEFT)] = true;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_LEFT)] = false;
    }
    break;
  case 's':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(MoveDirection::BACKWARD)] = true;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(MoveDirection::BACKWARD)] = false;
    break;
  case 'd':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = true;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = false;
    }
    break;
  case 'q':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_LEFT)] = true;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_LEFT)] = false;
    break;
  case 'e':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_RIGHT)] = true;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_RIGHT)] = false;
    break;
  case 'j':
    if (evtype == NCTYPE_PRESS) {
      // figure out new way to swtich debug plane rendering on/off
    }
    break;
  default:
    break;
  }
}
