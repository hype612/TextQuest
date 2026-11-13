#include "../Headers/NotcursesInputHandler.h"
#include "./Logger.h"
#include "EntityAction.h"
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

bool NotcursesInputHandler::keyDown(EntityAction dir) const {
  return _mvmtKeyStates[static_cast<int>(dir)];
}

bool NotcursesInputHandler::rawKeyPressed(uint32_t key) const {
  return _pressedThisFrame.contains(key);
}

void NotcursesInputHandler::KeyEvent(char in[], ncintype_e evtype) {
  switch (*in) {
  case 'w':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(EntityAction::FORWARD)] = true;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(EntityAction::FORWARD)] = false;
    }
    break;
  case 'a':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(EntityAction::STRAFE_LEFT)] = true;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(EntityAction::STRAFE_LEFT)] = false;
    }
    break;
  case 's':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(EntityAction::BACKWARD)] = true;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(EntityAction::BACKWARD)] = false;
    break;
  case 'd':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(EntityAction::STRAFE_RIGHT)] = true;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(EntityAction::STRAFE_RIGHT)] = false;
    }
    break;
  case 'j':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(EntityAction::TURN_LEFT)] = true;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(EntityAction::TURN_LEFT)] = false;
    break;
  case 'l':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(EntityAction::TURN_RIGHT)] = true;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(EntityAction::TURN_RIGHT)] = false;
    break;
  case 'q':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(EntityAction::TURN_LEFT)] = true;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(EntityAction::TURN_LEFT)] = false;
    break;
  case 'e':
    if (evtype == NCTYPE_PRESS)
      _mvmtKeyStates[static_cast<int>(EntityAction::TURN_RIGHT)] = true;
    if (evtype == NCTYPE_RELEASE)
      _mvmtKeyStates[static_cast<int>(EntityAction::TURN_RIGHT)] = false;
    break;
  case 'i':
    if (evtype == NCTYPE_PRESS) {
      _mvmtKeyStates[static_cast<int>(EntityAction::SHOOT)] = true;
    }
    if (evtype == NCTYPE_RELEASE) {
      _mvmtKeyStates[static_cast<int>(EntityAction::SHOOT)] = false;
    }
    break;
  default:
    break;
  }
}

void NotcursesInputHandler::resetInputState() {
  for (size_t i = 0; i < entityActionCount; i++) {
    _mvmtKeyStates[i] = false;
  }
  _pressedThisFrame.clear();
}
