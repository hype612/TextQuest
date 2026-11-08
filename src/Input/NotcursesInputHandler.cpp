#include "../Headers/NotcursesInputHandler.h"
#include "./Logger.h"
#include <notcurses/notcurses.h>
#include <string>

NotcursesInputHandler::NotcursesInputHandler(std::shared_ptr<notcurses> nc)
    : _nc(nc) {
  Logger::setLogLevel(LogLevel::INFO);
  for (size_t i = 0; i < _mvmtKeyStates.size(); i++) {
    _mvmtKeyStates[i] = false;
  }
}

void NotcursesInputHandler::Init() {}
void NotcursesInputHandler::ReceiveInput() {
  _pressedThisFrame.clear();
  ncinput in_char;
  timespec ts{0, 1000000};
  uint32_t rc;
  while ((rc = notcurses_get(_nc.get(), &ts, &in_char)) > 0) {
    // A release is not a press. Otherwise the release of the key that closed
    // one menu would count as a press in the next menu.
    if (in_char.evtype != NCTYPE_RELEASE)
      _pressedThisFrame.insert(in_char.id);
    KeyEvent(in_char.utf8, in_char.evtype);
  }
}

float NotcursesInputHandler::keyDown(MoveDirection dir) const {
  return _mvmtKeyStates[static_cast<int>(dir)];
}

bool NotcursesInputHandler::rawKeyPressed(uint32_t key) const {
  return _pressedThisFrame.contains(key);
}

void NotcursesInputHandler::KeyEvent(char in[], ncintype_e evtype) {
  switch (*in) {
  case 'w':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::FORWARD)] = 1.f;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::FORWARD)] = 0.f;
    }
    break;
  case 'a':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_LEFT)] = 1.f;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_LEFT)] = 0.f;
    }
    break;
  case 's':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(MoveDirection::BACKWARD)] = 1.f;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(MoveDirection::BACKWARD)] = 0.f;
    break;
  case 'd':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = 1.f;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = 0.f;
    }
    break;
  case 'j':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_LEFT)] = 1.f;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_LEFT)] = 0.f;
    break;
  case 'l':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_RIGHT)] = 1.f;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_RIGHT)] = 0.f;
    break;
  case 'q':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_LEFT)] = 1.f;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_LEFT)] = 0.f;
    break;
  case 'e':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_RIGHT)] = 1.f;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_RIGHT)] = 0.f;
    break;
  case 'i':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::SHOOT)] = 1.f;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::SHOOT)] = 0.f;
    }
    break;
  default:
    break;
  }
}
