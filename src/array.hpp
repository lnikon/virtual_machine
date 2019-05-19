#pragma once

#include "common.hpp"
#include "type_defs.hpp"

struct Array
{
  std::string         name_{};
  std::size_t         size_{};
  std::vector<QWORD>  value_{};
  ValueType           type_{};

  bool isInitialized{false};
};

