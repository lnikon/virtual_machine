#pragma once

#define DEBUG_MODE

#ifdef DEBUG_MODE
#include <iostream>
#include "logger.hpp"
#include "parser_defs.hpp"
#endif

inline void printStackSection(const StackSection& stackSec)
{
#ifdef DEBUG_MODE
  std::string stackInfo;
  stackInfo += "Stack Size: " + std::to_string(stackSec.size_) + "\n";

  Logger::printMessage(stackInfo, LogLevel::DEBUG);
#endif
}

