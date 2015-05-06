/*
  core/engine/serial/index.hpp - engine serial index management API source

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
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDED_INSILICO_CORE_ENGINE_SERIAL_INDEX_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_SERIAL_INDEX_HPP

#include "insilico/core/type.hpp"
#include "insilico/core/engine/data.hpp"
#include "insilico/core/engine/serial/common.hpp"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico { namespace engine {

auto neuron_index(unsigned _id, std::string _variable) -> unsigned {
  char key[128];
  sprintf(key, "n%d%s", _id, _variable.c_str());
  if(index_map.find(key) == index_map.end()) {
    std::cerr << "[insilico::engine::neuron_index] Failed to find "<<_variable
              <<" index for neuron "<<_id<<".\n";
    exit(1);
  }
  return index_map[key];
}

auto synapse_index(unsigned _id, std::string _variable) -> unsigned {
  char key[128];
  sprintf(key, "s%d%s", _id, _variable.c_str());
  if(index_map.find(key) == index_map.end()) {
    std::cerr << "[insilico::engine::synapse_index] Failed to find "<<_variable
              <<" index for synapse "<<_id<<".\n";
    exit(1);
  }
  return index_map[key];
}

auto neuron_index(unsigned _id, std::string _variable, bool& error)
    -> unsigned {
  char key[128];
  sprintf(key, "n%d%s", _id, _variable.c_str());
  if(index_map.find(key) == index_map.end()) {
    error = true;
    return 0;
  }
  return index_map[key];
}

auto synapse_index(unsigned _id, std::string _variable, bool& error)
    -> unsigned {
  char key[128];
  sprintf(key, "s%d%s", _id, _variable.c_str());
  if(index_map.find(key) == index_map.end()) {
    error = true;
    return 0;
  }
  return index_map[key];
}

auto get_neuron_indices(std::string _variable) -> std::vector<unsigned> {
  std::vector<unsigned> indices;
  bool error = false;
  unsigned idx;
  unsigned total_neurons = neuron_count();
  for(unsigned index = 0; index < total_neurons; ++index) {
    idx = neuron_index(index, _variable, error);
    if(!error) { indices.push_back(idx); }
    error = false;
  }
  return indices;
}

auto get_synapse_indices(std::string _variable) -> std::vector<unsigned> {
  std::vector<unsigned> indices;
  bool error = false;
  unsigned idx;
  unsigned total_synapses = synapse_count();
  for(unsigned index = 0; index < total_synapses; ++index) {
    idx = synapse_index(index, _variable, error);
    if(!error) { indices.push_back(idx); }
    error = false;
  }
  return indices;
}

auto get_indices(std::string _variable) -> std::vector<unsigned> {
  std::vector<unsigned> indices;
  indices.insert(indices.end(),
                 get_neuron_indices(_variable).begin(),
                 get_neuron_indices(_variable).end());
  indices.insert(indices.end(),
                 get_synapse_indices(_variable).begin(),
                 get_synapse_indices(_variable).end());
  return indices;
}

auto neuron_id_from_index(unsigned _index) -> unsigned {
  if(neuron_start_list_ids.empty() ||
     _index < neuron_start_list_ids.front() ||
     _index > neuron_end_list_ids.back())  {
    std::cerr << "[insilico::engine::neuron_id_from_index] "
              << "Failed to find index "<<_index <<'\n';
    exit(0);
  }
  auto it = std::lower_bound(neuron_start_list_ids.begin(),
                             neuron_start_list_ids.end(),
                             _index);  
  return (it - neuron_start_list_ids.begin());
}

auto neuron_id_from_index(unsigned _index, bool &error) -> unsigned {
  if(neuron_start_list_ids.empty() ||
     _index < neuron_start_list_ids.front() ||
     _index > neuron_end_list_ids.back())  {
    error = true;
  }
  auto it = std::lower_bound(neuron_start_list_ids.begin(),
                             neuron_start_list_ids.end(),
                             _index);  
  return (it - neuron_start_list_ids.begin());
}

auto synapse_id_from_index(unsigned _index) -> unsigned {
  if(synapse_start_list_ids.empty() ||
     _index < synapse_start_list_ids.front() ||
     _index > synapse_end_list_ids.back())  {
    std::cerr << "[insilico::engine::synapse_id_from_index] "
              << "Failed to find index "<<_index <<'\n';
    exit(0);
  }
  auto it = std::lower_bound(synapse_start_list_ids.begin(),
                             synapse_start_list_ids.end(),
                             _index);
  return (it - synapse_start_list_ids.begin());
}

auto synapse_id_from_index(unsigned _index, bool &error) -> unsigned {
  if(synapse_start_list_ids.empty() ||
     _index < synapse_start_list_ids.front() ||
     _index > synapse_end_list_ids.back())  {
    error = true;
  }
  auto it = std::lower_bound(synapse_start_list_ids.begin(),
                             synapse_start_list_ids.end(),
                             _index);
  return (it - synapse_start_list_ids.begin());
}

auto variable_name_from_index(unsigned _index) -> std::string {
  if(_index < var_list_ids.size()) {
    return var_list_ids[_index];
  }
  return "";
}

auto get_pre_neuron_indices(unsigned _id, std::string _variable)
    -> std::vector<unsigned> {
  std::vector<unsigned> indices;
  unsigned idx;
  bool error = false;
  if(!pre_synaptic_lists.empty()) {
    for(unsigned index = 0; index < pre_synaptic_lists[_id].size(); ++index) {
      idx = synapse_index(pre_synaptic_lists[_id][index], _variable, error);
      if(!error) {
        indices.push_back(idx);
      }
    }
  }
  return indices;
}

} } // namespace insilico::engine

#endif
