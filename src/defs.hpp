#pragma once
#include "common.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <variant>

/* Instruction names */
static const std::string NOP_INSTR_NAME = "NOP";
static const std::string ADD_INSTR_NAME = "ADD";

/* Instruction extensions */
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

/* Variable types */
using BYTE  = uint8_t;
using CHAR  = BYTE;
using WORD  = uint16_t;
using DWORD = uint32_t;
using QWORD = uint64_t;

enum class InstructionType 
{
  NOP = 0,
  ADD
};

struct Instruction
{
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

