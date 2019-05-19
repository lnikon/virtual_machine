#pragma once

#include "common.hpp"
#include "type_defs.hpp"

/* Types of operands */
enum class OperandType
{
  REG = 0,  // Register name
  FUN,      // Function name
  MEM,      // Memory Address
};

struct Operand
{
  QWORD       value_{};
  OperandType opType_{OperandType::REG};
  ValueType   valueType_{ValueType::DW};
};
using OpList = std::vector<Operand>;

