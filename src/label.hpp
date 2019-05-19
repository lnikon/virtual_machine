#pragma once

#include "common.hpp"
#include "instruction.hpp"

struct Label
{
  std::string     name_{};
  InstructionList code_{};
};
using LabelList = std::vector<Label>;

