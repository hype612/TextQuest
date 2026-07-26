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
  Logger::GetInstance()->log(
      "polling on nc at: " +
          std::to_string(reinterpret_cast<uintptr_t>(_nc.get())),
      LogType::INPUT, LogLevel::INFO);
  ncinput in_char;
  timespec ts{0, 1000000};
  uint32_t rc;
  Logger::GetInstance()->log("input as follows: ", LogType::INPUT,
                             LogLevel::INFO);
  while ((rc = notcurses_get(_nc.get(), &ts, &in_char)) > 0) {
    Logger::GetInstance()->log("got input", LogType::INPUT, LogLevel::INFO);
    Logger::GetInstance()->log(std::to_string(rc), LogType::INPUT,
                               LogLevel::INFO);
    KeyEvent(in_char.utf8, in_char.evtype);
  }
  /*
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
    _player.move(delta, MoveDirection::TURN_RIGHT); */
}

bool NotcursesInputHandler::keyDown(MoveDirection dir) const {
  Logger::GetInstance()->log("checking keydown on dir: " +
                                 std::to_string(static_cast<int>(dir)),
                             LogType::INPUT, LogLevel::INFO);
  return _mvmtKeyStates[static_cast<int>(dir)];
}

void NotcursesInputHandler::KeyEvent(char in[], ncintype_e evtype) {
  std::string dbg = "KeyEvent received: ";
  dbg += *in;
  Logger::GetInstance()->log(dbg, LogType::INPUT, LogLevel::INFO);
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
