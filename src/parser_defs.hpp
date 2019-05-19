#pragma once

#include <unordered_map>

#include "common.hpp"
#include "vm_config.hpp"
#include "variable.hpp"
#include "extension_defs.hpp"
#include "type_defs.hpp"
#include "instruction.hpp"
#include "operand.hpp"
#include "label.hpp"
#include "function.hpp"
#include "array.hpp"

struct StackSection
{
  std::size_t size_{VM_DEFAULT_STACK_SIZE};
};

/*
 * TODO: Pack into VariableEnvirnoment
 */
struct DataSection
{
  void insertVariable(const std::string& name, const Variable& var) {}
  const Variable& getVariable(const std::string& name) {}
  bool variableExists(const std::string& name) {}
  bool variableExists(const Variable& name) {}

  std::vector<Variable> getVariablesVector() const 
  {
    std::vector<Variable> vars;

    for(const auto var : variableMap)
    {
      vars.push_back(var.second);
    }

    return vars;
  }

  private:
  std::unordered_map<std::string, Variable> variableMap;  
};

/*
 * TODO: Pack into VariableEnvirnoment
 */
struct CodeSection
{
  FunctionList code_{};

  void insertFunction(const Function& function) {}
  bool isForwardDeclared(const std::string& name) {}
  bool functionExists(const std::string& name) {}
  bool functionExists(const Function& name) {}
};

struct MainSection
{
  std::string mainFunctionName_{};
};
