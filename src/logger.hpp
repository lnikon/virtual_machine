#include <iostream>
#include <string>

#pragma once

enum class LogLevel
{
  LOW = 0,
  MID,
  HIGH
};

struct Logger
{
  static void printMessage(const std::string& msg, LogLevel lvl = LogLevel::LOW);
};
