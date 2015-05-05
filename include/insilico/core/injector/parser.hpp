/*
 core/injector/parser.hpp - insilico Current injector's parsing
                            capability header and source

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

#ifndef INCLUDED_INSILICO_CORE_INJECTOR_PARSER_HPP
#define INCLUDED_INSILICO_CORE_INJECTOR_PARSER_HPP

#include "insilico/core/helper/file.hpp"
#include "insilico/core/helper/string.hpp"
#include "insilico/core/configuration/error.hpp"

#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

namespace insilico { namespace injector {

std::vector<std::vector<double>> external_current_seq;
std::vector<double> time_seq;
std::vector<unsigned> neurons_seq;

void read(const std::string &external_current_file) {
  bool header;
  char linedelim = '\n', worddelim = ',';
  std::string part;
  
  std::ifstream extcurrfile_stream(external_current_file);
  if(file_check(extcurrfile_stream, external_current_file)) {
    while(getline(extcurrfile_stream, part, linedelim)) { // read a line
      header = false;
      std::stringstream l(part);
      trim(part);
      if(part.length() > 0) {
        if(starts_with(part,"time")) {
          header = true;
        }
        std::vector<double> seq;
        while(getline(l, part, worddelim)) { // read the current
          trim(part);
          if(part.length() > 0) {
            if(header == true) {
              if(part.compare("time") == 0) continue;
              int neuron_id = (int)(ceil(string_to_double(part)));
              neurons_seq.push_back(neuron_id);
            }
            else {
              if(neurons_seq.empty()) {
                std::cerr << "[insilico::injector] External current input file "
                    "is incorrect or malformed.";
                configuration::severe_error();
              }
              seq.push_back(string_to_double(part));
            }
          }
        }
        if(!seq.empty()) {
          time_seq.push_back(seq[0]);
          external_current_seq.resize(neurons_seq.size());
          for(std::vector<unsigned>::size_type iter = 0;
              iter < neurons_seq.size(); ++iter) {
            external_current_seq[iter].push_back(seq[iter+1]);
          }
        }
      }
    }
  }
  extcurrfile_stream.close();
}

} } // namespace insilico::injector

#endif
