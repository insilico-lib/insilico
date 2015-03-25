/*
  core/engine/serial/value.hpp - insilico engine serial API header and source

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_SERIAL_VALUE_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_SERIAL_VALUE_HPP

#include "core/configuration/error.hpp"
#include "core/type.hpp"
#include "core/engine/data.hpp"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico { namespace engine {

auto neuron_value(int id, std::string variable, bool error=true) -> double {
  char key[128];
  sprintf(key, "n%d%s", id, variable.c_str());
  try {
    return value_map.at(key);
  }
  catch(...) {
    if(!error) {
      return 0;
    }
    else {
      std::cerr << "[insilico::engine::neuron_value] Failed to find "<<variable
                << " value for neuron " << id << ".\n";
      configuration::severe_error();
    }
  }
  return 0;
}

auto neuron_value(int id, std::string variable, double value) -> void {
  char key[128];
  sprintf(key, "n%d%s", id, variable.c_str());
  value_map[key] = value;
}

auto synapse_value(int id, std::string variable, bool error=true) -> double {
  char key[128];
  sprintf(key, "s%d%s", id, variable.c_str());
  try {
    return value_map.at(key);
  }
  catch(...) {
    if(!error) {
      return 0;
    }
    else {
      std::cerr << "[insilico::engine::synapse_index] Failed to find " << variable
                << " value for synapse " << id << ".\n";
      configuration::severe_error();
    }
  }
  return 0;
}

auto synapse_value(int id, std::string variable, double value) -> void {
  char key[128];
  sprintf(key, "s%d%s", id, variable.c_str());
  value_map[key] = value;
}

auto neuron_current_value(int id, std::string variable, bool error=true) -> double {
  char key[128];
  sprintf(key, "cn%d%s", id, variable.c_str());
  try {
    return value_map.at(key);
  }
  catch(...) {
    if(!error) {
      return 0;
    }
    else {
      std::cerr << "[insilico::engine::synapse_index] Failed to find " << variable
                << " value for synapse " << id << ".\n";
      configuration::severe_error();
    }
  }
  return 0;
}

auto neuron_current_value(int id, std::string variable, double value) -> void {
  char key[128];
  sprintf(key, "cn%d%s", id, variable.c_str());
  value_map[key] = value;
}

auto synapse_current_value(int id, std::string variable, bool error=true) -> double {
  char key[128];
  sprintf(key, "cs%d%s", id, variable.c_str());
  try {
    return value_map.at(key);
  }
  catch(...) {
    if(!error) {
      return 0;
    }
    else {
      std::cerr << "[insilico::engine::synapse_index] Failed to find " << variable
                << " value for synapse " << id << ".\n";
      configuration::severe_error();
    }
  }
  return 0;
}

auto synapse_current_value(int id, std::string variable, double value) -> void {
  char key[128];
  sprintf(key, "cs%d%s", id, variable.c_str());
  value_map[key] = value;
}

auto get_neuron_values(std::string variable) -> std::vector< double > {
  std::vector<double> values;
  int idx = -1;
  unsigned total_neurons = neuron_count();
  unsigned total_synapses = synapse_count();
  for(std::vector<double>::size_type index = 0; index < total_neurons; ++index) {
    idx = neuron_value(index, variable, false);
    if(idx >= 0) values.push_back(idx);
  }
  for(std::vector<double>::size_type index = 0; index < total_synapses; ++index) {
    idx = synapse_value(index, variable, false);
    if(idx >= 0) values.push_back(idx);
  }
  return values;
}

auto get_pre_neuron_values(int neuron_id, std::string variable) -> std::vector<double> {
  std::vector<double> values;
  if(!pre_synaptic_lists.empty()) {
    for(std::vector<double>::size_type index = 0; index < pre_synaptic_lists[neuron_id].size(); ++index) {
      values.push_back(synapse_value(pre_synaptic_lists[neuron_id][index], variable));
    }
  }
  return values;
}

} } // namespace insilico::engine

#endif
