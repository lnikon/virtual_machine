#include "lexer.hpp"
#include "parser.hpp"
#include "lexer_defs.hpp"
#include "utility.hpp"
#include "logger.hpp"
#include "debug.hpp"

StackSection Lexer::parseStackSection(std::fstream& inputStream)
{
  StackSection stackSec;

  bool isStackSizeOk = false;
  auto line = std::string{};

  while(!isStackSizeOk && std::getline(inputStream, line))
  {
    lineNumber_++;

    line = trim_copy(line);

    if(line.empty())
    {
      continue;
    }

    isStackSizeOk = parseStackSize(stackSec, line);
  }

  // If stack size hasn't been specified,
  // then set it to the default value
  if(!isStackSizeOk)
  {
    setDefaultStackSize(stackSec);
  }

  // Some debugging :)
  printStackSection(stackSec);

  return stackSec;
}

bool Lexer::parseStackSize(StackSection& rStackSec, std::string line)
{
  bool isStackSizeOk = false;

  const std::size_t posOfStack = line.find(STACK_SEC_NAME);  
  if(posOfStack != std::string::npos)
  {
    // Check assignment sign
    const std::size_t assignPos = line.find("=");
    if(assignPos == std::string::npos)
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Missing assignment sign\n", LogLevel::HIGH);
      isStackSizeOk = false;
      exit(1);
    }

    // Parse value of the stack size
    const auto stackSizeValueStr = trim_copy(line.substr(assignPos + 1, line.size() - assignPos));
    if(stackSizeValueStr.empty())
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Missing stack size value\n", LogLevel::HIGH);
      isStackSizeOk = false;
      exit(1);
    }

    bool isStackSizeValueCorrectNumber = is_number(stackSizeValueStr);
    if(!isStackSizeValueCorrectNumber)
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Stack size value should be non-negative integer\n", LogLevel::HIGH);
      isStackSizeOk = false;
      exit(1);
    }

    const auto stackSizeValueNum = std::stol(stackSizeValueStr);
    isStackSizeOk = true;
    rStackSec.size_ = stackSizeValueNum;
  }  

  return isStackSizeOk;
}

void Lexer::setDefaultStackSize(StackSection& rStackSec)
{
  Logger::printMessage("Value for stack size didn\'t specified. Defaulting to " + 
      std::to_string(VM_DEFAULT_STACK_SIZE) + ".\n", LogLevel::MID);

  rStackSec.size_ = VM_DEFAULT_STACK_SIZE;
}

DataSection Lexer::parseDataSection(std::fstream& inputStream)
{
  DataSection dataSec;

  auto line = std::string{};
  bool isDataSectionOk = false;
  bool isDataSectionPresent = false;

  std::streampos currentPos;
  std::streampos oldPos;
  while(std::getline(inputStream, line))
  {
    lineNumber_++;
    currentPos = inputStream.tellg();

    line = trim_copy(line);

    if(line.empty())
    {
      oldPos = currentPos;
      continue;
    }

    // If .DATA doesn't section present, 
    // then start processing of the .CODE section
    // else, read the next line
    const auto dataSectionPos = line.find(DATA_SEC_NAME);
    isDataSectionPresent = (dataSectionPos != std::string::npos);
    break;
  }

  if(isDataSectionPresent)
  {
    while(!isDataSectionOk && std::getline(inputStream, line))
    {
      lineNumber_++;

      bool isArrayDecl = isArrayDeclaration(line);          
      if(isArrayDecl)
      {
        parseArray(dataSec, line);
      }
      else
      {
        parseVariable(dataSec, line);
      }
    }
  }
  else
  {
    // In case absences of .DATA section go back to the
    // .CODE line
    inputStream.seekg(oldPos);
  }

  return dataSec;
}

void Lexer::parseVariable(DataSection& rDataSec, std::string line)
{

}

void Lexer::parseArray(DataSection& rDataSec, std::string line)
{
  Array array;
  array.type_ = getSpecifiedType(line);

}

bool Lexer::isArrayDeclaration(const std::string& line)
{
  return ((line.find("[") != std::string::npos) 
      && (line.find("]") != std::string::npos));
}

bool Lexer::isTypeSpecified(const std::string& line)
{
  const auto posOfSpaceBetweenTypeAndName = line.find_first_of(" ");
  if(posOfSpaceBetweenTypeAndName != std::string::npos)
  {
    std::string arrayType = line.substr(0, posOfSpaceBetweenTypeAndName);
    arrayType = trim_copy(arrayType);
    if(arrayType.empty())
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". No type specified for variable or array declaration.\n", LogLevel::HIGH);
      exit(1);
    }
  }

  return true;
}

ValueType Lexer::getSpecifiedType(const std::string& line)
{
  isTypeSpecified(line);
  return Parser::returnTypeForString(line);
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

