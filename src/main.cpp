#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

#include "defs.hpp"
#include "logger.hpp"

void printUsage()
{
  std::cout << "Usage: ./czpl <filename>.zpl\n";
}

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    printUsage();
    exit(1);
  }

  const std::string inputFilename = argv[1];
  std::fstream inputStream(inputFilename);
  if(!inputStream.is_open())
  {
    Logger::printMessage("Unable to open file " + inputFilename, LogLevel::HIGH);
  }
  
  return 0;
}
