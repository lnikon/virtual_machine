#include "logger.hpp"

void Logger::printMessage(const std::string& msg, LogLevel lvl)
{
  auto prefix = std::string{};
  switch(lvl)
  {
    case LogLevel::LOW:
      prefix = "Attention! ";
      break;
    case LogLevel::MID:
      prefix = "Warning! ";
      break;
    case LogLevel::HIGH:
      prefix = "Error! ";
      break;
    default: 
      break;
  }

  std::cout << prefix << msg << std::endl;
}
