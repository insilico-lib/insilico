/*
 core/helper/string.hpp - String manipulation helper header and source

 Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_HELPER_STRING_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_HELPER_STRING_HPP

#include <sstream>
#include <string>
#include <vector>

namespace insilico {

// trim string from beginning (left)
static inline std::string& ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim string from ending (right)
static inline std::string& rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// trim string from both the ends
static inline std::string& trim(std::string &s) {
  return ltrim(rtrim(s));
}

// convert string literal to double precision floating point number
static inline double string_to_double(std::string strnum) {
  double value;
  try {
    value = ::atof(strnum.c_str());
  }
  catch(const std::exception& e) {
    std::cout<<"[insilico::] Simulation Exception: "
             <<"supplied with file that contains improper value: "<< strnum <<'\n';
    exit(0);
  }
  return value;
}

// check if the string (subject) starts with given sub string (test)
bool inline starts_with(const std::string &subject, const std::string &test) {
  return (test.length() <= subject.length())  &&
      (equal(test.begin(), test.end(), subject.begin()));
}

// inplace string split 
void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

// split string and return vector
std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

} // namespace insilico

#endif
