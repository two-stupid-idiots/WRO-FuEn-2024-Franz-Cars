#include "Logger.h"
#include <Arduino.h>

void Logger::setLevel(LogLevel level) {
  this->level = level;
}

LogLevel Logger::getLevel(void) {
  return this->level;
}

void Logger::debug(const char* message) {
  Serial.print("[DEBUG] ");
  Serial.println(message);
}

void Logger::info(const char* message) {
  Serial.print("[INFO]  ");
  Serial.println(message);
}

void Logger::warn(const char* message) {
  Serial.print("[WARN]  ");
  Serial.println(message);
}

void Logger::error(const char* message) {
  Serial.print("[Error] ");
  Serial.println(message);
}

void Logger::fatal(const char* message) {
  Serial.print("[FATAL] ");
  Serial.println(message);
}

Logger logger;