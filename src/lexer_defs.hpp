// For the Syntax Specification of Zpl see src/syntax_description.txt

#pragma once

#include <string>
#include <vector>
#include <functional>

/* Instruction names */
const std::string NOP_INSTR_NAME = "NOP";
const std::string ADD_INSTR_NAME = "ADD";

/* Instruction extensions */
enum class Extension 
{
  B   = sizeof(uint8_t),
  W   = sizeof(uint16_t),
  DW  = sizeof(uint32_t),
  QW  = sizeof(uint64_t),
};

/* Variable types */
using BYTE  = uint8_t;
using CHAR  = BYTE;
using WORD  = uint16_t;
using DWORD = uint32_t;
using QWORD = uint64_t;

enum class InstructionType 
{
  NOP = 0,
  ADD,
};

enum class OperandType
{
  REG = 0,  // Register name
  FUN,      // Function name
  MEM,      // Memory Address
};

using ValueType = Extension;

struct Operand
{
  QWORD value_{};
  OperandType opType_{OperandType::REG};
  ValueType valueType_{ValueType::DW};
};

struct Instruction
{
  using OpList = std::vector<Operand>;

  std::string name_{};
  InstructionType type_{InstructionType::NOP};
  Extension ex_{Extension::DW};
  OpList opList_;
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

