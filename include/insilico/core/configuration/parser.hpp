/*
 core/configuration/parser.hpp - insilico's Configuration
                                 parser header and source

 Copyright (C) 2014-2015 Pranav Kulkarni, Collins Assisi Lab,
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
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_PARSER_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_PARSER_HPP

#include "insilico/core/helper/file.hpp"
#include "insilico/core/helper/string.hpp"

#include <sstream>
#include <string>

namespace insilico { namespace configuration {

// clean input file inputs
std::string remove_comments(std::string &with_comments) {
  std::stringstream s(with_comments);
  char delim = '\"';
  std::vector<std::string> full;
  std::string part;
  while(getline(s, part, delim)) {
    full.push_back(part);
  }
  with_comments = "";
  for(std::vector<std::string>::size_type it=0; it < full.size(); it+=2) {
    with_comments += full[it];
  }
  return with_comments;
}

// read the input files - neuron_file and synapse_file
//
// Both neuron and synapse files are coupled
// so parsers are placed in sync
// the variable 'ncount' is coupling variable.
// TODO: remove coupling of two different parsers
void read(const std::string neuron_file, const std::string synapse_file="") {
  int ntrack = 0, strack = 0, ncount = 0, dxdt_count = 0;
  double second_item = 0;
  char linedelim = ';', worddelim = ',', pairdelim = ':';

  std::string part, first_item, key;
  std::stringstream out;
  std::ifstream neuron_stream(neuron_file), synapse_stream(synapse_file);

  if(file_check(neuron_stream, neuron_file)) {
    while(getline(neuron_stream, part, linedelim)) {
      std::stringstream l(remove_comments(part));
      trim(part);
      if(part.length() > 0) {
        engine::neuron_start_list_ids.push_back(ncount);
        while(getline(l, part, worddelim)) {
          trim(part);
          if(part.length() > 0) {
            std::stringstream k(part);
            getline(k, part, pairdelim);
            trim(part);
            first_item = part;
            getline(k, part, pairdelim);
            trim(part);
            second_item = string_to_double(part);
            out.str("");
            out << ntrack;
            key = "n" + out.str() + first_item;
            if(first_item.compare("dxdt") == 0) {
              dxdt_count = (int)(ceil(second_item));
            }
            else if(dxdt_count > 0) {
              engine::var_list_ids.push_back(first_item);
              engine::var_vals.push_back(second_item);
              engine::index_map[key] = ncount;
              ++ncount;
              --dxdt_count;
            }
            // universal value map
            engine::value_map[key] = second_item;
          }
        }
        ntrack+=1;
        engine::neuron_end_list_ids.push_back(ncount);
      }
    }
  }
  neuron_stream.close();

  if(file_check(synapse_stream, synapse_file)) {
    while(getline(synapse_stream, part, linedelim)) {
      std::stringstream l(remove_comments(part));
      trim(part);
      if(part.length() > 0) {
        engine::synapse_start_list_ids.push_back(ncount);
        while(getline(l, part, worddelim)) {
          trim(part);
          if(part.length() > 0) {
            std::stringstream k(part);
            getline(k, part, pairdelim);
            trim(part);
            first_item = part;
            getline(k, part, pairdelim);
            trim(part);
            second_item = string_to_double(part);
            out.str("");
            out << strack;
            key = "s" + out.str() + first_item;
            if(first_item.compare("dxdt") == 0) {
              dxdt_count = (int)(ceil(second_item));
            }
            else if(first_item.compare("pre") == 0) {
              engine::pre_neuron.push_back((int)(ceil(second_item)));
            }
            else if(first_item.compare("post") == 0) {
              engine::post_neuron.push_back((int)(ceil(second_item)));
            }
            else if(dxdt_count > 0) {
              engine::var_list_ids.push_back(first_item);
              engine::var_vals.push_back(second_item);
              engine::index_map[key] = ncount;
              ++ncount;
              --dxdt_count;
            }
            // universal value map
            engine::value_map[key] = second_item;
          }
        }
        strack+=1;
        engine::synapse_end_list_ids.push_back(ncount);
      }
    }
    engine::populate_pre_synaptic_lists();
  }
  synapse_stream.close();
}

} } // namespace insilico::configuration

#endif
