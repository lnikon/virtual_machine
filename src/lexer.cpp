#include <utility>

#include "lexer.hpp"
#include "parser.hpp"
#include "lexer_defs.hpp"
#include "utility.hpp"
#include "logger.hpp"
#include "debug.hpp"

#define DEBUG_MODE

bool Lexer::checkCorrectKeyword(const std::string& line)
{
    if(line.empty())
    {
        return false;
    }

    if(!std::isalpha(line[0]))
    {
        return false;
    }

    for(std::size_t i = 0; i < line.size(); i++)
    {
        const unsigned char ch = static_cast<unsigned char>(line[i]);
        if(!std::isalnum(ch) && ch != '_')
        {
            return false;
        }
    }

    return true;
}

StackSection Lexer::parseStackSection(std::fstream& inputStream)
{
    StackSection stackSec;

    bool isStackSizeOk = false;
    auto line = std::string{};

    while(!isStackSizeOk && std::getline(inputStream, line))
    {
        lineNumber_++;

        line = trim_copy(line);

        if(line.empty())
        {
            continue;
        }

        isStackSizeOk = parseStackSize(stackSec, line);
    }

    // If stack size hasn't been specified,
    // then set it to the default value
    if(!isStackSizeOk)
    {
        setDefaultStackSize(stackSec);
    }

    // Some debugging :)
    printStackSection(stackSec);

    return stackSec;
}

bool Lexer::parseStackSize(StackSection& rStackSec, std::string line)
{
    bool isStackSizeOk = false;

    const std::size_t posOfStack = line.find(STACK_SEC_NAME);  
    if(posOfStack != std::string::npos)
    {
        // Check assignment sign
        const std::size_t assignPos = line.find("=");
        if(assignPos == std::string::npos)
        {
            Logger::printMessage("Syntax error on line " 
                    + std::to_string(lineNumber_) 
                    + ". Missing assignment sign\n", LogLevel::HIGH);
            isStackSizeOk = false;
            exit(1);
        }

        // Parse value of the stack size
        const auto stackSizeValueStr = trim_copy(line.substr(assignPos + 1, line.size() - assignPos));
        if(stackSizeValueStr.empty())
        {
            Logger::printMessage("Syntax error on line " 
                    + std::to_string(lineNumber_) 
                    + ". Missing stack size value\n", LogLevel::HIGH);
            isStackSizeOk = false;
            exit(1);
        }

        bool isStackSizeValueCorrectNumber = is_number(stackSizeValueStr);
        if(!isStackSizeValueCorrectNumber)
        {
            Logger::printMessage("Syntax error on line " 
                    + std::to_string(lineNumber_) 
                    + ". Stack size value should be non-negative integer\n", LogLevel::HIGH);
            isStackSizeOk = false;
            exit(1);
        }

        const auto stackSizeValueNum = std::stol(stackSizeValueStr);
        isStackSizeOk = true;
        rStackSec.size_ = stackSizeValueNum;
    }  

    return isStackSizeOk;
}

void Lexer::setDefaultStackSize(StackSection& rStackSec)
{
    Logger::printMessage("Value for stack size didn\'t specified. Defaulting to " + 
            std::to_string(VM_DEFAULT_STACK_SIZE) + ".\n", LogLevel::MID);

    rStackSec.size_ = VM_DEFAULT_STACK_SIZE;
}

DataSection Lexer::parseDataSection(std::fstream& inputStream)
{
    DataSection dataSec;

    auto line = std::string{};
    bool isDataSectionOk = false;
    bool isDataSectionPresent = false;


    while(!isDataSectionPresent && std::getline(inputStream, line))
    {
        lineNumber_++;

        line = trim_copy(line);

        if(line.empty())
        {
            continue;
        }

        // If .DATA doesn't section present, 
        // then start processing of the .CODE section
        // else, read the next line
        const auto dataSectionPos = line.find(DATA_SEC_NAME);
        isDataSectionPresent = (dataSectionPos != std::string::npos);
    }

    if(isDataSectionPresent)
    {
        while(!isDataSectionOk && std::getline(inputStream, line))
        {
            lineNumber_++;

            line = trim_copy(line);

            if(line.empty() || starts_with(line, "#"))
            {
                continue; 
            }

            const bool isCodeSegment = starts_with(line, CODE_SEC_NAME);
            const bool isMainSegment = starts_with(line, MAIN_SEC_NAME);
            if(isCodeSegment || isMainSegment)
            {
                break;
            }

            bool isArrayDecl = isArrayDeclaration(line);          
            if(isArrayDecl)
            {
                parseArray(dataSec, line);
            }
            else
            {
                parseVariable(dataSec, line);
            }
        }
    }

    return dataSec;
}

void Lexer::parseVariable(DataSection& rDataSec, std::string line)
{
    Variable variable;

    // Get variable type
    variable.type_ = getSpecifiedType(line);

    // Get variable name
    // TODO: Make variables and arrays use same functions for pasing
    variable.name_ = getVariableName(line); 

    // If variable used as 'string' then burn out the user.
    const bool isVariableUsedAsString = isStringDeclaration(line);
    if(isVariableUsedAsString)
    {
        Logger::printMessage("Syntax error on line " 
                + std::to_string(lineNumber_) 
                + ". Type mismatch. Use CHAR array instead.\n", LogLevel::HIGH);
        exit(1);
    }

    // Depending on the type of the variable
    // one of branches should be active
    const auto typeStr = Parser::returnStringForType(variable.type_);

    // This flag is for syntactical checks
    bool isAssignSignPresent{false};

    // If the variable is not char,
    // then parse its value as a string
    std::vector<std::string> valueFromLexer{};

    // If the variable type is CHAR,
    // then parse its value as char
    char charValueFromLexer;
    if(!typeStr.compare("CHAR"))
    {
        std::tie(isAssignSignPresent, charValueFromLexer) = getCharValueForLexer(line);
        variable.charValue_ = (char)charValueFromLexer;
    } 
    else 
    {
        std::tie(isAssignSignPresent, valueFromLexer) = getArrayValueForLexer(line);

        if(valueFromLexer.size() > 1)
        {
            Logger::printMessage("Syntax error on line " 
                    + std::to_string(lineNumber_) 
                    + ". Too many values specified for variable.\n", LogLevel::HIGH);
            exit(1);
        }

        variable.valueFromLexer_ = valueFromLexer[0];
    }

    rDataSec.insertVariable(variable.name_, variable);

    printVariable(variable);
}

void Lexer::parseArray(DataSection& rDataSec, std::string line)
{
    Array array;

    // Parse array type
    array.type_ = getSpecifiedType(line);

    // Parse array name
    // TODO: Check for name redifinition
    array.name_ = getArrayName(line);

    // Parse array size
    const auto [isSizeSpecified, size] = getArraySize(line);
    array.size_ = size;
    array.isSizeSpecified_ = isSizeSpecified;

    // Parse array value
  // If the array type is CHAR[] then interpret is as a 'string',
  // otherwise as a regular array 
  const auto typeStr = Parser::returnStringForType(array.type_);
  bool isAssignSignPresent{false};
  std::vector<std::string> valueFromLexer{};
  if(!typeStr.compare("CHAR"))
  {
    std::tie(isAssignSignPresent, valueFromLexer) = getStringValueForLexer(line);
  } 
  else 
  {
    std::tie(isAssignSignPresent, valueFromLexer) = getArrayValueForLexer(line);
  }

  // Set array values
  array.valueFromLexer_     = valueFromLexer;
  array.valueSizeFromLexer_ = array.valueFromLexer_.size();
  array.isInitialized_      = array.valueSizeFromLexer_ > 0;

  /*
   * Necessary syntactical checks
   */

  // If size and value were specified but assignment
  // sign is missing, then line is ill formed
  if(array.isSizeSpecified_ && !isAssignSignPresent)
  {
    if(array.valueSizeFromLexer_ > 0)
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Missing assignment sign.\n", LogLevel::HIGH);
      exit(1);
    }
  }

  // If size wasn't specified and initialization list is empty
  // then line is ill formed
  if(!array.isSizeSpecified_ && array.valueFromLexer_.empty())
  {
    Logger::printMessage("Syntax error on line " 
        + std::to_string(lineNumber_) 
        + ". Neither array size nor initialization list specified. GO TO HELL!.\n", LogLevel::HIGH);
    exit(1);
  }

  // Add array into DataSection
  rDataSec.insertArray(array.name_, array);
  printArray(array);
}

bool Lexer::isArrayDeclaration(const std::string& line) const
{
  return ((line.find("[") != std::string::npos) 
      && (line.find("]") != std::string::npos));
}

bool Lexer::isStringDeclaration(const std::string& line) const
{
  return ((line.find("\"") != std::string::npos) 
      && (line.find("\"") != std::string::npos));
}

std::string Lexer::isTypeSpecified(const std::string& line)
{
  const auto posOfSpaceBetweenTypeAndName = line.find_first_of(" ");
  std::string arrayType;

  if(posOfSpaceBetweenTypeAndName != std::string::npos)
  {
    arrayType = line.substr(0, posOfSpaceBetweenTypeAndName);
    arrayType = trim_copy(arrayType);

    if(arrayType.empty())
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". No type specified for variable or array declaration.\n", LogLevel::HIGH);
      exit(1);
    }
  }

  return arrayType;
}

ValueType Lexer::getSpecifiedType(const std::string& line)
{
  const auto type = isTypeSpecified(line);
  return Parser::returnTypeForString(type);
}

std::string Lexer::getArrayName(const std::string& line)
{
  const auto nameStart = line.find_first_of(" ") + 1;
  const auto nameEnd = line.find_first_of("[") - 1;

  auto arrayName = line.substr(nameStart, nameEnd - nameStart + 1);
  arrayName = trim_copy(arrayName);

  if(arrayName.empty())
  {
    Logger::printMessage("Syntax error on line " 
        + std::to_string(lineNumber_) 
        + ". Empty name specified for array.\n", LogLevel::HIGH);
    exit(1);
  }

  return arrayName;
}

std::string Lexer::getVariableName(const std::string& line)
{
  const auto nameStart = line.find_first_of(" ") + 1;
  const auto nameEnd = line.find_first_of("=") - 1;

  auto variableName = line.substr(nameStart, nameEnd - nameStart + 1);
  variableName = trim_copy(variableName);

  if(variableName.empty())
  {
    Logger::printMessage("Syntax error on line " 
        + std::to_string(lineNumber_) 
        + ". Empty name specified for array.\n", LogLevel::HIGH);
    exit(1);
  }

  return variableName;
}

std::pair<bool, std::vector<std::string>> Lexer::getArrayValueForLexer(const std::string& line)
{
  std::vector<std::string> values;

  const auto posOfAssignSign      = line.find_first_of("=");
  const bool isAssignSignPresent  = (posOfAssignSign != std::string::npos);

  std::size_t offset = posOfAssignSign;
  if(!isAssignSignPresent)
  {
    offset = line.find_first_of("]");
  }
  const auto valueStart = offset + 1;

  std::string valueStr = line.substr(valueStart, line.size() - valueStart); 
  valueStr = trim_copy(valueStr);

  if(isAssignSignPresent && valueStr.empty())
  {
    Logger::printMessage("Syntax error on line " 
        + std::to_string(lineNumber_) 
        + ". Wrong array declaration. Empty value specified after '=' sign.\n", LogLevel::HIGH);
    exit(1);
  }

  // TODO: Pack into trailingCharacterCheck(character) function
  if(valueStr.size() > 0 && valueStr.find_last_of(",") == valueStr.size() - 1)
  {
    Logger::printMessage("Syntax error on line " 
        + std::to_string(lineNumber_) 
        + ". Trailing \',\' at the end of the line.\n", LogLevel::HIGH);
    exit(1);
  }

  tokenize(valueStr, values);
  for(auto& value : values)
  {
    value = trim_copy(value);
  }

  const std::size_t commaCount = std::count(valueStr.begin(), valueStr.end(), ',');;
  if(values.size() - 1 != commaCount)
  {
    Logger::printMessage("Syntax error on line " 
        + std::to_string(lineNumber_) 
        + ". Values should be separated by \',\'.\n", LogLevel::HIGH);
    exit(1);
  }

  return std::make_pair(isAssignSignPresent, values);
}

  std::pair<bool, std::vector<std::string>>
Lexer::getStringValueForLexer(const std::string& line)
{
  std::vector<std::string> result;

  const auto posOfAssignSign      = line.find_first_of("=");
  const bool isAssignSignPresent  = (posOfAssignSign != std::string::npos);

  std::size_t valueEnd{0};
  std::size_t offset = posOfAssignSign;
  if(!isAssignSignPresent)
  {
    offset = line.find_first_of("]");
  }
  else
  {
    offset = line.find_first_of("\"");
    if(offset == std::string::npos)
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Missing opening \" in string value specification.\n", LogLevel::HIGH);
      exit(1);
    }

    valueEnd = line.find_last_of("\"");
    if(valueEnd == std::string::npos)
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Missing closing \" in string value specification.\n", LogLevel::HIGH);
      exit(1);
    }
    valueEnd -= 1;
  }
  const auto valueStart = offset + 1;

  std::string valueStr = line.substr(valueStart, valueEnd - valueStart); 
  valueStr = trim_copy(valueStr);

  result.push_back(valueStr);

  return std::make_pair(isAssignSignPresent, result);
}

std::pair<bool, char> Lexer::getCharValueForLexer(const std::string& line)
{
  char result;

  const auto posOfAssignSign      = line.find_first_of("=");
  const bool isAssignSignPresent  = (posOfAssignSign != std::string::npos);

  std::size_t valueEnd{0};
  std::size_t offset = posOfAssignSign;
  if(!isAssignSignPresent)
  {
    // WARNING: This is temporary solution
    std::size_t whitespacePos = line.find_first_of(" ");
    if(whitespacePos == std::string::npos)
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Wrong declaration of variable.\n", LogLevel::HIGH);
      exit(1);
    }

    std::size_t nameStart = whitespacePos + 1;
    while(line[nameStart] == ' ') 
    {
      nameStart++;
    }

    whitespacePos = line.find_first_of(' ', whitespacePos); 
    offset = whitespacePos;
  }
  else
  {
    offset = line.find_first_of("\'");
    if(offset == std::string::npos)
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Missing opening \' in character value specification.\n", LogLevel::HIGH);
      exit(1);
    }

    valueEnd = line.find_last_of("\'");
    if(valueEnd == std::string::npos)
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Missing closing \' in character value specification.\n", LogLevel::HIGH);
      exit(1);
    }
    valueEnd -= 1;
  }
  const auto valueStart = offset + 1;

  std::string valueStr = line.substr(valueStart, valueEnd - valueStart + 1); 

  if(isAssignSignPresent && valueStr.size() != 1)
  {
    Logger::printMessage("Syntax error on line " 
        + std::to_string(lineNumber_) 
        + ". Invalid value specified for char.\n", LogLevel::HIGH);
    exit(1);
  }

  result = valueStr[0];

  return std::make_pair(isAssignSignPresent, result);
}

std::pair<bool, std::size_t> Lexer::getArraySize(const std::string& line)
{
  std::size_t size{0};
  bool isSizeSpecified{false};

  const std::size_t startOfSize = line.find_first_of("[") + 1;
  const std::size_t endOfSize   = line.find_first_of("]") - 1;

  std::string sizeStr = line.substr(startOfSize, endOfSize - startOfSize + 1);
  sizeStr = trim_copy(sizeStr);

  if(!sizeStr.empty())
  {
    if(!is_number(sizeStr))
    {
      Logger::printMessage("Syntax error on line " 
          + std::to_string(lineNumber_) 
          + ". Size of array should be non-negative integer.\n", LogLevel::HIGH);
      exit(1);
    } 

    size = std::stoul(sizeStr);
    isSizeSpecified = true;
  }

  return std::make_pair(isSizeSpecified, size);
}

static int counter = 0;

CodeSection Lexer::parseCodeSection(std::fstream& inputStream)
{
  CodeSection codeSec;

  bool isCodeSectionPresent   = false;
  bool isCodeSectionOk        = false;
  auto line = std::string{};

  while(!isCodeSectionPresent && std::getline(inputStream, line))
  {
    lineNumber_++;

    line = trim_copy(line);

    if(line.empty())
    {
      continue;
    }

    // If section .DATA doesn't present, 
    // then start processing of the .CODE section
    // else, read the next line
    const auto codeSectionPos = line.find(CODE_SEC_NAME);
    isCodeSectionPresent = (codeSectionPos != std::string::npos);
  }

  if(!isCodeSectionPresent)
  {
    Logger::printMessage("Syntax error on line " 
        + std::to_string(lineNumber_) 
        + ". CODE section should contain at least one function for the main.\n", LogLevel::HIGH);
    exit(1);
  }

  while(!isCodeSectionOk && std::getline(inputStream, line))
  {
    lineNumber_++;

    line = trim_copy(line);

    if(line.empty() || starts_with(line, "#"))
    {
      continue; 
    }

    const bool isMainSection = starts_with(line, MAIN_SEC_NAME);
    if(isMainSection)
    {
      break;
    }

    auto name = getFunctionName(line);
    // TODO: Check for function name redifinition.
    Function func;

    parseFunctionBody(inputStream, func);
  }

  return codeSec;
}

std::pair<bool, std::string> Lexer::getFunctionName(const std::string& line)
{
    auto [isFuncDecl, funcName] = isFunctionDeclaration(line);
    if(!isFuncDecl && functionCount_ < 1)
    {
        Logger::printMessage("Syntax error on line " 
                + std::to_string(lineNumber_) 
                + ". Function declaration parsing failed.\n", LogLevel::HIGH);
        exit(1);
    }

    return std::make_pair(isFuncDecl, funcName);
}

std::pair<bool, std::string> Lexer::isFunctionDeclaration(const std::string& line)
{
    bool startWithFunctionKeyword = starts_with(line, FUNCTION_KWRD);
    if(!startWithFunctionKeyword)
    {
        Logger::printMessage("Syntax error on line " 
                + std::to_string(lineNumber_) 
                + ". CODE section should start with function declaration.\n", LogLevel::HIGH);
        exit(1);
    }

    std::vector<std::string> tokens;
    tokenize(line, tokens);
    for(auto& token : tokens)
    {
        token = trim_copy(token);
    }

    if(tokens.size() > 3)
    {
        Logger::printMessage("Syntax error on line " 
                + std::to_string(lineNumber_) 
                + ". Too many tokens in function declaration.\n", LogLevel::HIGH);
        exit(1);
    }

    if(tokens.size() < 2)
    {
        Logger::printMessage("Syntax error on line " 
                + std::to_string(lineNumber_) 
                + ". Missing function name.\n", LogLevel::HIGH);
        exit(1);
    }

    auto funcName = tokens[1];
    funcName = trim_copy(funcName);

    if(funcName[funcName.size() - 1] == ':')
    {
       funcName = funcName.substr(0, funcName.size() - 1);
    }

    if(!checkCorrectKeyword(funcName))
    {

        Logger::printMessage("Syntax error on line " 
                + std::to_string(lineNumber_) 
                + ". Missing function name.\n", LogLevel::HIGH);
        exit(1);

    }

    const bool isFuncDecl = startWithFunctionKeyword && ((tokens.size() == 2) || (tokens.size() == 3));
    // std::cout << "startWithFunctionKeyword: " << std::boolalpha << startWithFunctionKeyword << std::endl;
    // std::cout << "isFuncDecl: " << std::boolalpha << isFuncDecl << std::endl;
    // std::cout << "tokens.size: " << tokens.size() << std::endl;
    if(isFuncDecl) 
    {
        functionCount_++;
        // std::cout << "functionCount_ = " << functionCount_ << "\n";
    }

    return std::make_pair(isFuncDecl, funcName);
}

void Lexer::parseFunctionBody(std::fstream& inputStream, Function& rFunc)
{
    auto line = std::string{};
    while(std::getline(inputStream, line))
    {
        lineNumber_++;

        line = trim_copy(line);

        if(line.empty() || starts_with(line, "#"))
        {
            continue; 
        }

        if(auto [isLabelParam, label] = isLabel(line); isLabelParam)
        {
            std::cout << "isLabelParam: " << isLabelParam << std::endl;
            std::cout << "lbl.name_: " << label.name_ << std::endl;

            rFunc.labels_.push_back(label);
        }
        else if(auto [isInstrParam, instr] = isInstruction(line); isInstrParam)
        {
            std::cout << "inInstrParam: " << isInstrParam << std::endl;
            std::cout << "instr.name_: " << instr.name_ << std::endl;

            rFunc.code_.push_back(instr);
        }
        else if(endOfFunctionDecl(line))
        {
            break;
        }
        else
        {
            Logger::printMessage("Syntax error on line " 
                    + std::to_string(lineNumber_) 
                    + ". Invalid statemant at function definition.\n", LogLevel::HIGH);
            exit(1);
        }
    }
}

std::pair<bool, Label> Lexer::isLabel(const std::string& line)
{
    auto label = Label{};
    auto isLabelParam = false;

    auto tokens = std::vector<std::string>{};
    tokenize(line, tokens);

    // Label declaration can consist of maximum 2 tokens
    if(tokens.size() > 2)
    {
        isLabelParam = false;
        return std::make_pair(isLabelParam, label);
    }

    auto name = std::string{}; 
    if(tokens.size() == 2)
    {
        name = tokens[0];
        name = trim_copy(name);

        isLabelParam = true;
    }
    else if(tokens.size() == 1)
    {
        name = tokens[0];
        name = trim_copy(name);

        if(name[name.size() - 1] != ':')
        {
            isLabelParam = false;
            return std::make_pair(isLabelParam, label);
        }

        name = name.substr(0, name.size() - 1);

        if(!checkCorrectKeyword(name))
        {
            Logger::printMessage("Syntax error on line " 
                    + std::to_string(lineNumber_) 
                    + ". Invalid identifier name for label.\n", LogLevel::HIGH);
            exit(1);

        }

        isLabelParam = true;
    }

    label.name_ = name;

    return std::make_pair(isLabelParam, label);
}

std::pair<bool, Instruction> Lexer::isInstruction(const std::string& line)
{
    bool isInstr = false;
    
    return std::make_pair(isInstr, Instruction{});
}

bool Lexer::endOfFunctionDecl(const std::string& line)
{
    return !(line.compare(END_FUNCTION_KWRD));
}

MainSection Lexer::parseMainSection(std::fstream& inputStream)
{
    MainSection mainSec;

    return mainSec;
}

void Lexer::parseMainFunction(MainSection& rMainSec)
{
}

