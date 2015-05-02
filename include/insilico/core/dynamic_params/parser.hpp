/*
 core/dynamic_params/parser.hpp - insilico dynamic parameters' parsing

 Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab,
                    IISER, Pune <pranavcode@gmail.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDED_INSILICO_CORE_DYNAMIC_PARAMS_PARSER_HPP
#define INCLUDED_INSILICO_CORE_DYNAMIC_PARAMS_PARSER_HPP

#include "insilico/core/helper/file.hpp"
#include "insilico/core/helper/string.hpp"
#include "insilico/core/configuration/error.hpp"

#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace insilico { namespace dynamic_params {

std::unordered_map<std::string, std::vector<double>> dynamic_params_seq;
std::vector<double> time_seq;
std::vector<std::string> params_seq;

void read(const std::string &_param_file) {
  bool header;
  char linedelim = '\n', worddelim = ',';
  std::string part;
  std::vector<double> seq;
  std::ifstream param_file_stream(_param_file);
  if(file_check(param_file_stream, _param_file)) {
    while(getline(param_file_stream, part, linedelim)) { // read a line
      header = false;
      std::stringstream l(part);
      trim(part);
      if(part.length() > 0) {
        if(starts_with(part,"time")) {
          header = true;
        }
        seq.clear();
        while(getline(l, part, worddelim)) { // read the param
          trim(part);
          if(part.length() > 0) {
            if(header == true) {
              if(part.compare("time") == 0) continue;
              params_seq.push_back(part);
            }
            else {
              if(params_seq.empty()) {
                std::cerr << "[insilico::dynamic_params]"
                    "Dynamic Parameters input file"
                    "is incorrect or malformed.\n";
                configuration::severe_error();
              }
              seq.push_back(string_to_double(part));
            }
          }
        }
        if(!seq.empty()) {
          time_seq.push_back(seq[0]);
          dynamic_params_seq[params_seq.back()].push_back(seq.back());
        }
      }
    }
  }
  param_file_stream.close();
}

} } // namespace insilico::dynamic_params

#endif
