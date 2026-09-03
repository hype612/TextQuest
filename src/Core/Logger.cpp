#include "../Headers/Logger.h"
#include <ios>

Logger *Logger::GetInstance() {
  if (instancePtr == nullptr) {
    instancePtr = new Logger();
  }
  return instancePtr;
}

void Logger::log(const std::string &msg, LogType type, LogLevel lv) {
  if (lv < _logLv) {
    return;
  }
  switch (type) {
  case LogType::CORE:
    _commonLog << "CORE:" << lv << ": " << msg << std::endl;
    _coreLog << msg << std::endl;
    break;
  case LogType::INPUT:
    _commonLog << "INPUT:" << lv << ": " << msg << std::endl;
    _inputLog << msg << std::endl;
    break;
  case LogType::RENDER:
    _commonLog << "RENDER:" << lv << ": " << msg << std::endl;
    _renderLog << msg << std::endl;
    break;
  case LogType::TEXPREP:
    _commonLog << "TEXPREP:" << lv << ": " << msg << std::endl;
    _texprepLog << msg << std::endl;
    break;
  case LogType::SCENE:
    _commonLog << "SCENE:" << lv << ": " << msg << std::endl;
    _sceneLog << msg << std::endl;
    break;
  }
}

void Logger::setLogLevel(LogLevel newLv) { _logLv = newLv; }

void Logger::forceFlush() {
  _commonLog.flush();
  _coreLog.flush();
  _inputLog.flush();
  _renderLog.flush();
  _texprepLog.flush();
  _sceneLog.flush();
}

Logger::~Logger() {
  _commonLog.flush();
  _coreLog.flush();
  _inputLog.flush();
  _renderLog.flush();
  _texprepLog.flush();

  _commonLog.close();
  _coreLog.close();
  _inputLog.close();
  _renderLog.close();
  _texprepLog.close();
  _sceneLog.close();
  delete instancePtr;
  instancePtr = nullptr;
}

Logger *Logger::instancePtr = nullptr;
LogLevel Logger::_logLv = LogLevel::INFO;
std::ofstream Logger::_commonLog("./logs/common.log", std::ios_base::trunc);
std::ofstream Logger::_coreLog("./logs/core.log", std::ios_base::trunc);
std::ofstream Logger::_inputLog("./logs/input.log", std::ios_base::trunc);
std::ofstream Logger::_renderLog("./logs/render.log", std::ios_base::trunc);
std::ofstream Logger::_texprepLog("./logs/texprep.log", std::ios_base::trunc);
std::ofstream Logger::_sceneLog("./logs/scene.log", std::ios_base::trunc);
