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
  ncinput in_char;
  timespec ts{0, 1000000};
  uint32_t rc;
  while ((rc = notcurses_get(_nc.get(), &ts, &in_char)) > 0) {
    KeyEvent(in_char.utf8, in_char.evtype);
  }
}

bool NotcursesInputHandler::keyDown(MoveDirection dir) const {
  return _mvmtKeyStates[static_cast<int>(dir)];
}

bool NotcursesInputHandler::quitPressed() const { return _quitPressed; }

void NotcursesInputHandler::KeyEvent(char in[], ncintype_e evtype) {
  std::string dbg = "KeyEvent received: ";
  dbg += *in;
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
  case 'x':
    if (evtype == NCTYPE_PRESS) {
      _quitPressed = true;
    }
    break;
  case 'i':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::SHOOT)] = true;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::SHOOT)] = false;
    }
    break;
  default:
    break;
  }
}
