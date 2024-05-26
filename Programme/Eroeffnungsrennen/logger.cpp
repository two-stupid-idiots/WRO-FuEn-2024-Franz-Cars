#include "Logger.h"

void Logger::setLevel(LogLevel level) {
  this->level = level;
}

LogLevel Logger::getLevel(void) {
  return this->level;
}

void Logger::debug(const char* message) {
  if (level <= LogLevel::DEBUG)
  {
    Serial.print("[DEBUG] ");
    Serial.println(message);
  }
}

void Logger::info(const char* message) {
  if (level <= LogLevel::INFO)
  {
    Serial.print("[INFO]  ");
    Serial.println(message);
  }
}

void Logger::warn(const char* message) {
  if (level <= LogLevel::WARN)
  {
    Serial.print("[WARN]  ");
    Serial.println(message);
  }
}

void Logger::error(const char* message) {
  if (level <= LogLevel::ERROR)
  {
    Serial.print("[ERROR] ");
    Serial.println(message);
  }
}

void Logger::fatal(const char* message) {
  if (level <= LogLevel::FATAL)
  {
    Serial.print("[FATAL] ");
    Serial.println(message);
  }
}

Logger logger;