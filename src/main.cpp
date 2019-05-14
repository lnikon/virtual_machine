#include <iostream>

#include "defs.hpp"

int main()
{
  uint16_t value = 0;
  std::cin >> value;

  Arg arg;
  auto variantValue = Arg::ValueType{value};
  arg.setValue(variantValue);

  std::visit(InstructionAdd{}, arg.value_, arg.value_);

  return 0;
}
