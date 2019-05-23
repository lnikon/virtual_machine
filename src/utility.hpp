#pragma once

#include <algorithm>
#include <cctype>
#include <locale>

#include "common.hpp"

// Copied from StackOverflow
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

void find_and_replace(std::string& str,
                      const std::string& oldStr,
                      const std::string& newStr)
{
  std::string::size_type pos = 0u;
  while((pos = str.find(oldStr, pos)) != std::string::npos){
    str.replace(pos, oldStr.length(), newStr);
    pos += newStr.length();
  }
}

//! Tokenize the given string str with given delimiter. If no delimiter is given whitespace is used.
void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ")
{
  tokens.clear();
  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}

std::string convertBoolToString(const bool value)
{
  return (value) ? std::string{"true"} : std::string{"false"};
}

std::string convertVectorOfStringToString(const std::vector<std::string>& strings)
{
  std::string result;
  result += "[";
  for(const auto& str : strings)
  {
    result += str + " ";
  }

  if(result.size() > 1)
  {
    result[result.size() - 1] = ']';
  }
  else
  {
    result += ']';
  }

  return result;
}

bool starts_with(const std::string& line, const std::string& prefix)
{
  return (line.rfind(prefix, 0) == 0);
}
