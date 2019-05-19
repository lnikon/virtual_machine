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
  //StackSection  stackSec_{};
  //DataSection   dataSec_{};
  //CodeSection   codeSec_{};
  //MainSection   mainSec_{};

  /*
   * Stack section parsing routines
   */
  void parseStackSize(StackSection& rStackSec);

  /*
   * Data section parsing routines 
   */
  void parseVariable(DataSection& rDataSec);
  void parseArray(DataSection& rDataSec);

  /*
   * Code section parsing routines
   */
  void parseFunction(CodeSection& rCodeSec);

  /*
   * Main section parsing routines
   */
  void parseMainFunction(MainSection& rMainSec);
};
