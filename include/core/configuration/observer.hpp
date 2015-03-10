/*
 core/configuration/observer.hpp - Simulation output observer

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

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_OBSERVER_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_OBSERVER_HPP

#include "core/configuration/error.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace insilico { namespace configuration {

std::vector< std::string > observation_header;
std::vector< int > pre_computed_indices;
std::ofstream outstream;

bool header_observed_flag = false;

auto write_header_once() -> void {
  if(!header_observed_flag && !observation_header.empty()) {
    outstream << "time";
    for(std::string _variable : observation_header) {
      outstream << ',' << _variable;
    }
    outstream << '\n';
    header_observed_flag = true;
  }
}

struct observer {
  auto operator() (state_type &variables, const double t) -> void {
    write_header_once();
    outstream << t;
    for(std::vector< double >::size_type index = 0; index < pre_computed_indices.size(); ++index) {
      outstream << ',' << (variables[pre_computed_indices[index]]);
    }
    outstream << '\n';
  }
};

auto observe(std::string _variable) -> void {
  std::vector< int > neuron_indices = engine::get_neuron_indices(_variable);
  std::vector< int > synapse_indices = engine::get_synapse_indices(_variable);
  char key[128];
  if(neuron_indices.empty() && synapse_indices.empty()) {
    std::cerr << "[insilico::configuration] Observer failed to find " << _variable << ".\n";
    configuration::severe_error();
  }
  if(!neuron_indices.empty()) {
    pre_computed_indices.insert(pre_computed_indices.end(), neuron_indices.begin(), neuron_indices.end());
    for(int index : neuron_indices) {
      sprintf(key, "n%d%s", engine::neuron_id_from_index(index), _variable.c_str());
      observation_header.push_back(key);
    }
  }
  if(!synapse_indices.empty()) {
    pre_computed_indices.insert(pre_computed_indices.end(), synapse_indices.begin(), synapse_indices.end());    
    for(int index : synapse_indices) {
      sprintf(key, "s%d%s", engine::synapse_id_from_index(index), _variable.c_str());
      observation_header.push_back(key);
    }
  }
}

auto observe_neuron(std::string _variable, int id) -> void {
  int neuron_index = engine::neuron_index(id, _variable, false);
  char key[128];
  if(neuron_index == -1) {
    std::cerr << "[insilico::configuration] Observer failed to find " << _variable
              << " for Neuron " << id << ".\n";
    configuration::severe_error();
  }
  else {
    pre_computed_indices.push_back(neuron_index);
    sprintf(key, "n%d%s", id, _variable.c_str());
    observation_header.push_back(key);
  }
}

auto observe_neuron(std::string _variable, std::vector< int > ids) -> void {
  std::vector< int > neuron_indices;
  char key[128];
  int neuron_index;
  for(int id : ids) {
    neuron_index = engine::neuron_index(id, _variable, false);
    if(neuron_index != -1) {
      neuron_indices.push_back(neuron_index);
    }
  }
  if(neuron_indices.empty()) {
    std::cerr << "[insilico::configuration] Observer failed to find " << _variable
              << " for Neurons ID list supplied.\n";
    configuration::severe_error();
  }
  else {
    pre_computed_indices.insert(pre_computed_indices.end(), neuron_indices.begin(), neuron_indices.end());
    for(int index : neuron_indices) {
      sprintf(key, "n%d%s", engine::neuron_id_from_index(index), _variable.c_str());
      observation_header.push_back(key);
    }
  }
}

auto observe_synapse(std::string _variable, int id) -> void {
  int synapse_index = engine::synapse_index(id, _variable, false);
  char key[128];
  if(synapse_index == -1) {
    std::cerr << "[insilico::configuration] Observer failed to find " << _variable
              << " for Synapse " << id << ".\n";
    configuration::severe_error();
  }
  else {
    pre_computed_indices.push_back(synapse_index);
    sprintf(key, "s%d%s", id, _variable.c_str());
    observation_header.push_back(key);
  }
}

auto observe_synapse(std::string _variable, std::vector< int > ids) -> void {
  std::vector< int > synapse_indices;
  char key[128];
  int synapse_index;
  for(int id : ids) {
    synapse_index = engine::synapse_index(id, _variable, false);
    if(synapse_index != -1) {
      synapse_indices.push_back(synapse_index);
    }
  }
  if(synapse_indices.empty()) {
    std::cerr << "[insilico::configuration] Observer failed to find " << _variable
              << " for Synapses ID list supplied.\n";
    configuration::severe_error();
  }
  else {
    pre_computed_indices.insert(pre_computed_indices.end(), synapse_indices.begin(), synapse_indices.end());
    for(int index : synapse_indices) {
      sprintf(key, "s%d%s", engine::synapse_id_from_index(index), _variable.c_str());
      observation_header.push_back(key);
    }
  }
}

} } // namespace insilico::configuration

#endif
