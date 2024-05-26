#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

enum class LogLevel {
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL
};

class Logger {
  public:
    void setLevel(LogLevel level);
    LogLevel getLevel(void);
    void debug(const char* message);
    void info(const char* message);
    void warn(const char* message);
    void error(const char* message);
    void fatal(const char* message);

  private:
    LogLevel level = LogLevel::INFO;
};

extern Logger logger;

#endif