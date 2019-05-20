// For the language specification of Zpl see src/syntax_description.txt
#pragma once

#include <fstream>

#include "common.hpp"
#include "parser_defs.hpp"

struct Lexer
{
  //void parse(const std::string& inputFilename);
  StackSection parseStackSection(std::fstream& inputStream);
  DataSection  parseDataSection(std::fstream& inputStream);
  CodeSection  parseCodeSection(std::fstream& inputStream);
  MainSection  parseMainSection(std::fstream& inputStream);

  private:
  std::size_t lineNumber_{0};
  //StackSection  stackSec_{};
  //DataSection   dataSec_{};
  //CodeSection   codeSec_{};
  //MainSection   mainSec_{};

  /*
   * Stack section parsing routines
   */
  bool parseStackSize(StackSection& rStackSec, std::string line);
  void setDefaultStackSize(StackSection& rStackSec);

  /*
   * Data section parsing routines 
   */
  void parseVariable(DataSection& rDataSec, std::string line);
  void parseArray(DataSection& rDataSec, std::string line);
  bool isArrayDeclaration(const std::string& line);
  bool isTypeSpecified(const std::string& line);
  ValueType getSpecifiedType(const std::string& line);

  /*
   * Code section parsing routines
   */
  void parseFunction(CodeSection& rCodeSec);
  // void parseInstruction();

  /*
   * Main section parsing routines
   */
  void parseMainFunction(MainSection& rMainSec);
};
