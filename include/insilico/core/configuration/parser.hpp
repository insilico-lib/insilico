/*
 core/configuration/parser.hpp - insilico's Configuration files parser

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
/**
 * @file core/configuration/parser.hpp
 *
 * Parser for insilico Configuration's input files.
 */

// TODO: The two parser (Neuron and Synapse) are strongly coupled due to parsing logic,
// needs a work to improve design and reduce duplicate code. Should be done ASAP.
// Will help in reducing complexity of parser as well.

#pragma once

#include "insilico/core/helper/file.hpp"
#include "insilico/core/helper/string.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace insilico {
namespace configuration {

/**
 * Removes comments from a string,
 * default comment starts and ends with double quote.
 *
 * @param with_comments Ref to string with comment(s).
 */
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

/**
 * Parses or reads and stores the input configurations for
 * Neurons and Synapses in the simulation.
 *
 * @param neuron_file name of Neuron configuration input file.
 * @param synapse_file name of Synapse confgiguration input file,
 *                     empty if not supplied as argument.
 */
void read(const std::string neuron_file, const std::string synapse_file="") {
  // Helps in keeping track of number of Neurons.
  int ntrack = 0;

  // Helps in keeping track of number of Synapses.
  int strack = 0;

  // Keeps the ID for Neurons and Synapses for mapping into map keys.
  int ncount = 0;

  // Keeps the track of upcoming dxdt variables.
  int dxdt_count = 0;

  // Input file delimiters for Line, Word and Key value pair.
  char linedelim = ';', worddelim = ',', pairdelim = ':';

  // Temporary string.
  std::string part;

  // First half of key:value pair - key.
  std::string first_item;

  // Second half of key:value pair - value.
  double second_item = 0;

  // Unique key for storing key:values in map, generated at run-time.
  std::string key;

  // Buffered temporary helper string stream for generating keys.
  std::stringstream out;

  // Input file streams for Neuron and Synapse configuration files.
  std::ifstream neuron_stream(neuron_file), synapse_stream(synapse_file);

  // Parser for Neuron input file.
  if(file_check(neuron_stream, neuron_file)) {
    // For each line in input file.
    while(getline(neuron_stream, part, linedelim)) {
      // remove comments and create string stream.
      std::stringstream l(remove_comments(part));
      trim(part); // trim out spaces.
      if(part.length() > 0) {
        engine::neuron_start_list_ids.push_back(ncount);
        // For each key value pair.
        while(getline(l, part, worddelim)) {
          trim(part); // trim out spaces.
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
               // marks the start of dxdt variables.
              dxdt_count = (int)(ceil(second_item));
            }
            else if(dxdt_count > 0) {
              // if the current variable is part of dxdt.
              engine::prepopulated_neuron_ids.push_back(ntrack);
              engine::var_list_ids.push_back(first_item);
              engine::var_vals.push_back(second_item);
              engine::index_map[key] = ncount;
              ++ncount;
              --dxdt_count;
            }
            // universal value map.
            engine::value_map[key] = second_item;
          }
        }
        ntrack+=1;
        engine::neuron_end_list_ids.push_back(ncount);
      }
    }
  }
  neuron_stream.close();

  // Parser for Synapse input file.
  if(file_check(synapse_stream, synapse_file)) {
    // For each line in input file.
    while(getline(synapse_stream, part, linedelim)) {
      // remove comments and create string stream
      std::stringstream l(remove_comments(part));
      trim(part); // trim out spaces.
      if(part.length() > 0) {
        engine::synapse_start_list_ids.push_back(ncount);
        // For each key value pair.
        while(getline(l, part, worddelim)) {
          trim(part); // trim out spaces.
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
              // marks the start of dxdt variables.
              dxdt_count = (int)(ceil(second_item));
            }
            else if(first_item.compare("pre") == 0) {
              engine::pre_neuron.push_back((int)(ceil(second_item)));
            }
            else if(first_item.compare("post") == 0) {
              engine::post_neuron.push_back((int)(ceil(second_item)));
            }
            else if(dxdt_count > 0) {
              // if the current variable is part of dxdt.
              engine::prepopulated_synapse_ids.push_back(strack);
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

} // namespace configuration
} // namespace insilico
