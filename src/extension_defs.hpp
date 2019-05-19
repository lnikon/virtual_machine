#pragma once
#include "common.hpp"

/* Instruction extensions */
enum class Extension 
{
  INVALID = 0,
  B   = sizeof(uint8_t),
  W   = sizeof(uint16_t),
  DW  = sizeof(uint32_t),
  QW  = sizeof(uint64_t),
};

