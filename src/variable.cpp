#include "variable.hpp"

bool Variable::areEqual(const Variable& var1, const Variable& var2)
{
  const bool sameName   = (var1.name_ == var2.name_);
  const bool sameValue  = (var1.type_ == var2.type_);
  const bool sameType   = (var1.type_ == var2.type_);

  return sameName && sameValue && sameType; 
}
