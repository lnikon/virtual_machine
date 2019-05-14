#pragma once
#include "common.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <variant>

static const std::string ADD_INSTR_NAME = "ADD";

enum class Extension 
{
  B   = sizeof(uint8_t),
  W   = sizeof(uint16_t),
  DW  = sizeof(uint32_t),
  QW  = sizeof(uint64_t),
};

enum class ArgType
{
  UNKOWN = 0,
  BYTE,
  CHAR = BYTE,
  WORD,
  DWORD,
  QWORD,
};

// Types of variables
using BYTE  = uint8_t;
using CHAR  = BYTE;
using WORD  = uint16_t;
using DWORD = uint32_t;
using QWORD = uint64_t;

struct Arg
{
  using ValueType = std::variant<BYTE, WORD, DWORD, QWORD>;

  ArgType type_{ArgType::DWORD};
  ValueType value_;

  void setValue(const ValueType& value)
  {
    // 1. Update type of the argument 
    // according to the valid index of the passed value
    // 2. If the if doesn't successed - abort execution. DON'T TOUCH VALUE!!!
    // 3. Else, if type is being updated successfully, then update the value
    
    const bool isUpdateOk = updateType(value);
    if(!isUpdateOk) {
#ifdef DEBUG_MODE
      // TODO: Pass string to the logger
      std::cerr << "Unable to update Arg type\n";
#endif
      return;
    }

    value_ = value;
  }

  private:
  bool updateType(const ValueType& value)
  {
    const ArgType argType = getArgTypeByValueIndex(value);

    if(!isArgTypeValid(argType)) {
      return false;
    }

    type_ = argType;

    return true;
  } 

  ArgType getArgTypeByValueIndex(const ValueType& value)
  {
    const auto index = value.index();
    switch(index)
    {
      case 0:
        return ArgType::BYTE;
      case 1:
        return ArgType::WORD;
      case 2:
        return ArgType::DWORD;
      case 3:
        return ArgType::QWORD;
      default:
        return ArgType::UNKOWN;
    }
  }

  bool isArgTypeValid(const ArgType type)
  {
    switch(type) 
    {
      case ArgType::BYTE:
      case ArgType::WORD:
      case ArgType::DWORD:
      case ArgType::QWORD:
        return true;
      case ArgType::UNKOWN:
        return false;
      default:
        return false;
    }
  }
};

enum class InstructionType 
{
  OP_NOP = 0,
  OP_RETURN = 1, 
};

struct Instruction
{
  std::string name_{};
  Extension ex_{Extension::DW};
  Arg result_;
  std::vector<Arg> args_;
};

struct InstructionNop : Instruction {};
struct InstructionAdd : Instruction 
{
  
  void operator()(BYTE op1, BYTE op2)
  {
    ex_ = Extension::B;
    BYTE result = op1 + op2;
    Arg argResult;
    argResult.value_ = result;
    result_ = argResult;
  }

  void operator()(WORD op1, WORD op2)
  {
    ex_ = Extension::W;
    BYTE result = op1 + op2;
    Arg argResult;
    argResult.value_ = result;
    result_ = argResult;
  }

  void operator()(DWORD op1, DWORD op2)
  {
    ex_ = Extension::DW;
    BYTE result = op1 + op2;
    Arg argResult;
    argResult.value_ = result;
    result_ = argResult;
  }

  void operator()(QWORD op1, QWORD op2)
  {
    ex_ = Extension::QW;
    BYTE result = op1 + op2;
    Arg argResult;
    argResult.value_ = result;
    result_ = argResult;
  }

  InstructionAdd() 
  {
    name_ = ADD_INSTR_NAME;
  }
  
};

struct Function
{
};

struct Label
{
};

struct Variable
{
};

struct Array
{
};

