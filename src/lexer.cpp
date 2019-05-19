#include "lexer.hpp"

StackSection Lexer::parseStackSection(std::fstream& inputStream)
{
  StackSection stackSec;

  return stackSec;
}

void Lexer::parseStackSize(StackSection& rStackSec)
{
}

DataSection Lexer::parseDataSection(std::fstream& inputStream)
{
  DataSection dataSec;

  return dataSec;
}

void Lexer::parseVariable(DataSection& rDataSec)
{
}

void Lexer::parseArray(DataSection& rDataSec)
{
}

CodeSection Lexer::parseCodeSection(std::fstream& inputStream)
{
  CodeSection codeSec;

  return codeSec;
}

void Lexer::parseFunction(CodeSection& rCodeSec)
{
}

MainSection Lexer::parseMainSection(std::fstream& inputStream)
{
  MainSection mainSec;

  return mainSec;
}

void Lexer::parseMainFunction(MainSection& rMainSec)
{
}

