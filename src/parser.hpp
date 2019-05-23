#pragma once

#include <memory>

#include "common.hpp"
#include "lexer.hpp"
#include "parser_defs.hpp"

struct Parser
{
  Parser();
  void parse(const std::string& inputFilename);

  StackSection   getStackSection() const;
  DataSection    getDataSection()  const;
  CodeSection    getCodeSection()  const;
  MainSection    getMainSection()  const;

  static ValueType returnTypeForString(const std::string& type);
  static std::string returnStringForType(const ValueType type);

  private:
  using LexerPtr = std::unique_ptr<Lexer>;
  LexerPtr      pLexer_{nullptr};

  StackSection  stackSec_{};
  DataSection   dataSec_{};
  CodeSection   codeSec_{};
  MainSection   mainSec_{};

  /*
   * Utilities for semantic checks
   */

  /*
   * Check StackSection semantics
   */
  bool checkStackSection(const StackSection& stackSec)  const;
  bool checkStackSize(const std::size_t stackSize)      const;

  /*
   * Check DataSection semantics
   */
  bool checkDataSection(const DataSection& dataSec)       const;
  bool staticTypeCheck(const DataSection& dataSec)        const;
  bool isExpessionTypeSupported(ValueType type)           const;

};
