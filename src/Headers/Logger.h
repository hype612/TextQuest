#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

enum class LogType { CORE, INPUT, RENDER, TEXPREP };

enum LogLevel { INFO = 0, WARNING, ERROR };

class Logger {
public:
  Logger(const Logger &obj) = delete;

  static Logger *GetInstance();
  static void log(const std::string &msg, LogType type, LogLevel lv);
  static void setLogLevel(LogLevel newLv);
  static void forceFlush();
  ~Logger();

private:
  Logger() {}
  static Logger *instancePtr;
  static LogLevel _logLv;
  static std::ofstream _commonLog;
  static std::ofstream _coreLog;
  static std::ofstream _inputLog;
  static std::ofstream _renderLog;
  static std::ofstream _texprepLog;
};

#endif // LOGGER_H
