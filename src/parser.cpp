#include "common.hpp"
#include "vm_config.hpp"
#include "logger.hpp"
#include "parser.hpp"

Parser::Parser()
{
  pLexer_ = std::make_unique<Lexer>();
}

void Parser::parse(const std::string& inputFilename)
{
  std::fstream inputStream(inputFilename);
  if(!inputStream.is_open())
  {
    Logger::printMessage("Unable to open file " + inputFilename, LogLevel::HIGH);
    exit(1);
  }

  StackSection stackSec = pLexer_->parseStackSection(inputStream);
  const bool isStackSectionOk = checkStackSection(stackSec);
  if(!isStackSectionOk)
  {
    Logger::printMessage("StackSection is incorrect. Please review your code.\n", LogLevel::HIGH);
    exit(1);
  }
  stackSec_ = stackSec;

  DataSection dataSec = pLexer_->parseDataSection(inputStream);
  const bool isDataSectionOk = checkDataSection(dataSec);
  if(!isDataSectionOk)
  {
    Logger::printMessage("DataSection is incorrect. Please review your code.\n", LogLevel::HIGH);
    exit(1);
  }
  dataSec_ = dataSec;

  CodeSection codeSec = pLexer_->parseCodeSection(inputStream);
  codeSec_ = codeSec;

  MainSection mainSec = pLexer_->parseMainSection(inputStream);
  mainSec_ = mainSec;
}

StackSection Parser::getStackSection() const
{
  return stackSec_;
}

DataSection Parser::getDataSection() const
{
  return dataSec_;
}

CodeSection Parser::getCodeSection() const
{
  return codeSec_;
}

MainSection Parser::getMainSection() const
{
  return mainSec_;
}

bool Parser::checkStackSection(const StackSection& stackSec) const
{
  bool isAllOk = true;

  // Check stack size
  const std::size_t stackSize = stackSec.size_;
  const bool stackSizeOk = checkStackSize(stackSize);

  if(!stackSizeOk)
  {
    Logger::printMessage("Incorrect stack size: " + std::to_string(stackSize), LogLevel::HIGH);
  }

  isAllOk = stackSizeOk;  // && with other check flags
  return isAllOk;
}

bool Parser::checkStackSize(const std::size_t stackSize) const
{
  return (stackSize < VM_MAX_STACK_SIZE);
}

bool Parser::checkDataSection(const DataSection& dataSec) const
{
  bool isAllOk = true;

  const bool isStaticTypeCheckOk = staticTypeCheck(dataSec); 
  isAllOk = isStaticTypeCheckOk;

  return isAllOk;
}

bool Parser::staticTypeCheck(const DataSection& dataSec) const
{
  bool isStaticTypeCheckOk = true;

  auto vars = dataSec.getVariablesVector();
  for(const auto& var : vars)
  {
    const bool isVarTypeSupported = isExpessionTypeSupported(var.type_);
    if(!isVarTypeSupported)
    {
      Logger::printMessage("Variable " + var.name_ + " has unsupported type.\n", LogLevel::HIGH);
      exit(1);
    }

    isStaticTypeCheckOk = isVarTypeSupported; // do AND with others
  }

  return isStaticTypeCheckOk;
}

bool Parser::isExpessionTypeSupported(ValueType type) const
{
  switch(type)
  {
    case ValueType::B:
    case ValueType::W:
    case ValueType::DW:
    case ValueType::QW:
      return true;
    case ValueType::INVALID:
    default:
      return false;
  }
}

ValueType Parser::returnTypeForString(const std::string& type)
{
  if(type == "CHAR" || type == "BYTE") 
  {
    return ValueType::B;
  }
  else if(type == "WORD")
  {
    return ValueType::W;
  }
  else if(type == "DWORD")
  {
    return ValueType::DW;
  }
  else if(type == "QWORD")
  {
    return ValueType::QW;
  }
  else
  {
    return ValueType::INVALID;
  }
}
