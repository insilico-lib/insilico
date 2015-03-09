/*
  core/engine.hpp - insilico's Simulation Engine API header and source

  Copyright (C) 2014-2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_HPP

#include "core/global_type.hpp"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico { namespace engine {

std::unordered_map< std::string, int > index_map;
std::unordered_map< std::string, double > value_map;
std::vector< int > neuron_start_list_ids;
std::vector< int > neuron_end_list_ids;
std::vector< int > synapse_start_list_ids;
std::vector< int > synapse_end_list_ids;
std::vector< std::vector<int> > pre_synaptic_lists;
std::vector< int > pre_neuron;
std::vector< int > post_neuron;
std::vector< std::string > var_list_ids;
std::vector< double > var_vals;

std::vector<double> get_variables() {
  return var_vals;
}

inline int neuron_count() {
  return neuron_start_list_ids.size();
}

inline int synapse_count() {
  return synapse_start_list_ids.size();
}

int neuron_index(int id, std::string variable, bool error=true) {
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
      std::cout << "[insilico::engine::neuron_value] Failed to find "<<variable
                <<" index for neuron "<<id<<".\n";
      exit(0);
    }
  }
}

double neuron_value(int id, std::string variable, bool error=true) {
  char key[128];
  sprintf(key, "n%d%s", id, variable.c_str());
  try {
    return value_map.at(key);
  }
  catch(...) {
    if(!error) {
      return -1;
    }
    else {
      std::cout << "[insilico::engine::neuron_value] Failed to find "<<variable
                <<" value for neuron "<<id<<".\n";
      exit(0);
    }
  }
}

void neuron_value(int id, std::string variable, double value) {
  char key[128];
  sprintf(key, "n%d%s", id, variable.c_str());
  value_map[key] = value;
}

int synapse_index(int id, std::string variable, bool error=true) {
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
      std::cout << "[insilico::engine::synapse_index] Failed to find "<<variable
                <<" index for synapse "<<id<<".\n";
      exit(0);
    }
  }
}

double synapse_value(int id, std::string variable, bool error=true) {
  char key[128];
  sprintf(key, "s%d%s", id, variable.c_str());
  try {
    return value_map.at(key);
  }
  catch(...) {
    if(!error) {
      return -1;
    }
    else {
      std::cout << "[insilico::engine::synapse_index] Failed to find "<<variable
                <<" value for synapse "<<id<<".\n";
      exit(0);
    }
  }
}

void synapse_value(int id, std::string variable, double value) {
  char key[128];
  sprintf(key, "s%d%s", id, variable.c_str());
  value_map[key] = value;
}

double current_value(int id, std::string variable) {
  char key[128];
  sprintf(key, "c%d%s", id, variable.c_str());
  return value_map.at(key);
}

void current_value(int id, std::string variable, double value) {
  char key[128];
  sprintf(key, "c%d%s", id, variable.c_str());
  value_map[key] = value;
}

auto get_indices(std::string variable) -> std::vector<int> {
  std::vector<int> indices;
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

auto get_values(std::string variable) -> std::vector<double> {
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

std::vector<int> get_pre_neuron_indices(int neuron_id, std::string variable) {
  std::vector<int> indices;
  if(!pre_synaptic_lists.empty()) {
    for(std::vector<int>::size_type index = 0; index < pre_synaptic_lists[neuron_id].size(); ++index) {
      indices.push_back(synapse_index(pre_synaptic_lists[neuron_id][index], variable));
    }
  }
  return indices;
}

std::vector<double> get_pre_neuron_values(int neuron_id, std::string variable) {
  std::vector<double> values;
  if(!pre_synaptic_lists.empty()) {
    for(std::vector<double>::size_type index = 0; index < pre_synaptic_lists[neuron_id].size(); ++index) {
      values.push_back(synapse_value(pre_synaptic_lists[neuron_id][index], variable));
    }
  }
  return values;
}

void populate_pre_synaptic_lists() {
  if(!post_neuron.empty()) {
    pre_synaptic_lists.resize( *max_element(post_neuron.begin(), post_neuron.end()) + 1 );
    for(std::vector<int>::size_type iterator = 0; iterator < post_neuron.size(); ++iterator) {
      pre_synaptic_lists[ post_neuron[iterator] ].push_back( iterator );
    }
  }
}

class driver {
 public:
  void operator()(std::vector<double> &variables, std::vector<double> &dxdt, const double time);
};

} } // namespace insilico::engine

#endif
