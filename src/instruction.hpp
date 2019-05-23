#pragma once

#include "common.hpp"
#include "extension_defs.hpp"
#include "operand.hpp"

/* Instruction names */
const std::string NOP_INSTR_NAME = "NOP";
const std::string ADD_INSTR_NAME = "ADD";

/* Supported instructions */
enum class InstructionType 
{
  NOP = 0,
  ADD,
};

struct Instruction
{
  std::string     name_{};
  InstructionType type_{InstructionType::NOP};
  Extension       ex_{Extension::DWORD};
  OpList          opList_;
};
using InstructionList = std::vector<Instruction>;

