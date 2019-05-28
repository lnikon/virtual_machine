#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

#include "defs.hpp"
#include "logger.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "instruction_envirnoment.hpp"

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

  Parser parser;
  parser.parse(inputFilename);

  return 0;
}
