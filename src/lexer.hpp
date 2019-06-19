// For the language specification of Zpl see src/syntax_description.txt
#pragma once

#include <fstream>

#include "common.hpp"
#include "parser_defs.hpp"
#include "code_section_parser.hpp"

struct Lexer
{
  //void parse(const std::string& inputFilename);
  StackSection parseStackSection(std::fstream& inputStream);
  DataSection  parseDataSection(std::fstream& inputStream);
  CodeSection  parseCodeSection(std::fstream& inputStream);
  MainSection  parseMainSection(std::fstream& inputStream);

  private:
  /*
   * Parsers for general sections
   */
  CodeSectionParser codeSectionParser_{};

  std::size_t lineNumber_{0};
  std::size_t functionCount_{0};

  /*
   * Name checking utilities.
   */
//  bool checkCorrectKeyword(const std::string& line);

  /*
   * Stack section parsing routines
   */
  bool parseStackSize(StackSection& rStackSec, std::string line);
  void setDefaultStackSize(StackSection& rStackSec);

  /*
   * Data section parsing routines 
   */
  void                          parseVariable(DataSection& rDataSec, std::string line);
  void                          parseArray(DataSection& rDataSec, std::string line);
  bool                          isArrayDeclaration(const std::string& line) const;
  bool                          isStringDeclaration(const std::string& line) const;
  std::string                   isTypeSpecified(const std::string& line);
  ValueType                     getSpecifiedType(const std::string& line);
  std::string                   getArrayName(const std::string& line);
  std::string                   getVariableName(const std::string& line);

  std::pair<bool, std::vector<std::string>>
    getArrayValueForLexer(const std::string& line);

  std::pair<bool, std::vector<std::string>>
    getStringValueForLexer(const std::string& line);

  std::pair<bool, char>         getCharValueForLexer(const std::string& line);

  std::pair<bool, std::size_t>  getArraySize(const std::string& line);

  /*
   * Main section parsing routines
   */
  void parseMainFunction(MainSection& rMainSec);
};
