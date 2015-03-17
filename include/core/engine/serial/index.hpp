/*
  core/engine/serial/index.hpp - engine serial index management API header and source

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_SERIAL_INDEX_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_SERIAL_INDEX_HPP

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

auto neuron_index(int id, std::string variable, bool error=true) -> unsigned {
  char key[128];
  sprintf(key, "n%d%s", id, variable.c_str());
  try {
    return index_map.at(key);
  }
  catch(...) {
    if(!error) {
      return -1;
    }
    else {
      std::cerr << "[insilico::engine::neuron_index] Failed to find "<<variable
                <<" index for neuron "<<id<<".\n";
      configuration::severe_error();
    }
  }
  return 0;
}

auto synapse_index(int id, std::string variable, bool error=true) -> unsigned {
  char key[128];
  sprintf(key, "s%d%s", id, variable.c_str());
  try {
    return index_map.at(key);
  }
  catch(...) {
    if(!error) {
      return -1;
    }
    else {
      std::cerr << "[insilico::engine::synapse_index] Failed to find "<<variable
                <<" index for synapse "<<id<<".\n";
      configuration::severe_error();
    }
  }
  return 0;
}

auto get_indices(std::string variable) -> std::vector< unsigned > {
  std::vector< unsigned > indices;
  int idx = -1;
  unsigned total_neurons = neuron_count();
  unsigned total_synapses = synapse_count();
  for(std::vector<int>::size_type index = 0; index < total_neurons; ++index) {
    idx = neuron_index(index, variable, false);
    if(idx >= 0) indices.push_back(idx);
  }
  for(std::vector<int>::size_type index = 0; index < total_synapses; ++index) {
    idx = synapse_index(index, variable, false);
    if(idx >= 0) indices.push_back(idx);
  }
  return indices;
}

auto get_neuron_indices(std::string variable) -> std::vector< unsigned > {
  std::vector< unsigned > indices;
  int idx = -1;
  unsigned total_neurons = neuron_count();
  for(std::vector<int>::size_type index = 0; index < total_neurons; ++index) {
    idx = neuron_index(index, variable, false);
    if(idx >= 0) indices.push_back(idx);
  }
  return indices;
}

auto get_synapse_indices(std::string variable) -> std::vector< unsigned > {
  std::vector< unsigned > indices;
  int idx = -1;
  unsigned total_synapses = synapse_count();
  for(std::vector<int>::size_type index = 0; index < total_synapses; ++index) {
    idx = synapse_index(index, variable, false);
    if(idx >= 0) indices.push_back(idx);
  }
  return indices;
}

auto neuron_id_from_index(int _index) -> unsigned {
  unsigned idx = 0;
  while(_index > neuron_start_list_ids[idx] && idx < neuron_start_list_ids.size()) { ++idx; }
  return idx;
}

auto synapse_id_from_index(int _index) -> unsigned {
  unsigned idx = 0;
  while(_index > synapse_start_list_ids[idx] && idx < synapse_start_list_ids.size()) { ++idx; }
  return idx;
}

auto get_pre_neuron_indices(int neuron_id, std::string variable) -> std::vector<int> {
  std::vector<int> indices;
  if(!pre_synaptic_lists.empty()) {
    for(std::vector<int>::size_type index = 0; index < pre_synaptic_lists[neuron_id].size(); ++index) {
      indices.push_back(synapse_index(pre_synaptic_lists[neuron_id][index], variable));
    }
  }
  return indices;
}

} } // namespace insilico::engine

#endif
