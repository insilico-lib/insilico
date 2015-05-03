/*
 core/configuration/observer.hpp - Simulation output observer

 Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune
                    <pranavcode@gmail.com>
 Copyright (C) 2015 Anup G Pillai, Suhita Nadkarni Lab, IISER, Pune
                    <anupgpillai@gmail.com>
                    
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

#include "insilico/core/configuration/error.hpp"
#include "insilico/core/engine/serial/common.hpp"
#include "insilico/core/engine/serial/index.hpp"
#include "insilico/core/engine/serial/value.hpp"
#include "insilico/core/type.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace insilico { namespace configuration {

std::vector< std::string > observation_header;
std::vector< std::string > pre_computed_keys;
std::vector< int > pre_computed_indices;
std::ofstream outstream;
bool header_observed_flag = false;

// Observer defaults
char observer_delimiter = ',';
bool observer_header = true;

auto observe_delimiter(const char _delim) -> void {
  observer_delimiter = _delim;
}

auto observe_header(const bool _flag) -> void {
  observer_header = _flag;
}

auto write_header_once() -> void {
  if(observer_header && !header_observed_flag && !observation_header.empty()) {
    outstream << "time";
    for(std::string _variable : observation_header) {
      outstream << observer_delimiter << _variable;
    }
    outstream << '\n';
    header_observed_flag = true;
  }
}

struct observer {
  auto operator() (state_type &variables, const double t) -> void {
    write_header_once();
    outstream << t;
    for(unsigned id : pre_computed_indices) {
      outstream << observer_delimiter << (variables[id]);
    }
    for(std::string key : pre_computed_keys) {
      outstream << observer_delimiter << (engine::value_map[key]);
    }
    outstream << '\n';
  }
};

auto observe(std::string _variable) -> void {
  char key[128];
  bool error = false;
  std::vector< unsigned > neuron_indices;
  std::vector< unsigned > synapse_indices;
  std::vector< std::string > neuron_keys;
  std::vector< std::string > synapse_keys;
  neuron_indices = engine::get_neuron_indices(_variable);
  synapse_indices = engine::get_synapse_indices(_variable);
  neuron_keys = engine::get_neuron_value_keys(_variable);
  synapse_keys = engine::get_synapse_value_keys(_variable);
  if(neuron_indices.empty() && synapse_indices.empty() &&
     neuron_keys.empty() && synapse_keys.empty()) {
    std::cerr << "[insilico::configuration] Observer failed to find "
              << _variable << ".\n";
    configuration::severe_error();
  }
  if(!neuron_indices.empty()) {
    pre_computed_indices.insert(pre_computed_indices.end(),
                                neuron_indices.begin(),
                                neuron_indices.end());
    for(unsigned index : neuron_indices) {
      unsigned id = engine::neuron_id_from_index(index, error);
      if(!error) {
        sprintf(key, "n%d%s", id, _variable.c_str());
        observation_header.push_back(key);
      }
    }
  }
  else if(!neuron_keys.empty()) {
    pre_computed_keys.insert(pre_computed_keys.end(),
                             neuron_keys.begin(),
                             neuron_keys.end());
    for(std::string value_key : neuron_keys) {
      observation_header.push_back(value_key);
    }
  }
  if(!synapse_indices.empty()) {
    pre_computed_indices.insert(pre_computed_indices.end(),
                                synapse_indices.begin(),
                                synapse_indices.end());
    for(unsigned index : synapse_indices) {
      unsigned id = engine::synapse_id_from_index(index, error);
      if(!error) {
        sprintf(key, "s%d%s", id, _variable.c_str());
        observation_header.push_back(key);
      }
    }
  }
  else if(!synapse_keys.empty()) {
    pre_computed_keys.insert(pre_computed_keys.end(),
                             synapse_keys.begin(),
                             synapse_keys.end());
    for(std::string value_key : neuron_keys) {
      observation_header.push_back(value_key);
    }
  }
  if(error) {
    std::cerr << "[insilico::configuration] Observer failed to find "
              << _variable << ".\n";
    configuration::severe_error();
  }
}

auto observe_neuron(unsigned id, std::string _variable) -> void {
  bool error_index = false;
  int neuron_index = engine::neuron_index(id, _variable, error_index);
  std::string neuron_key = engine::neuron_value_key(id, _variable);
  char key[128];
  if(error_index && (neuron_key.compare("") == 0)) {
    std::cerr << "[insilico::configuration] Observer failed to find "
              << _variable << " for Neuron " << id << ".\n";
    configuration::severe_error();
  }
  else if(!error_index) {
    pre_computed_indices.push_back(neuron_index);
    sprintf(key, "n%d%s", id, _variable.c_str());
    observation_header.push_back(key);
  }
  else {
    pre_computed_keys.push_back(neuron_key);
    observation_header.push_back(neuron_key);
  }
}

auto observe_neuron(std::vector<unsigned> ids, std::string _variable) -> void {
  for(unsigned id : ids) {
    observe_neuron(id, _variable);
  }
}

auto observe_synapse(unsigned id, std::string _variable) -> void {
  bool error_index = false;
  int synapse_index = engine::synapse_index(id, _variable, error_index);
  std::string synapse_key = engine::synapse_value_key(id, _variable);
  char key[128];
  if(error_index && (synapse_key.compare("") == 0)) {
    std::cerr << "[insilico::configuration] Observer failed to find "
              << _variable << " for Synapse " << id << ".\n";
    configuration::severe_error();
  }
  else if(!error_index) {
    pre_computed_indices.push_back(synapse_index);
    sprintf(key, "s%d%s", id, _variable.c_str());
    observation_header.push_back(key);
  }
  else {
    pre_computed_keys.push_back(synapse_key);
    observation_header.push_back(synapse_key);
  }
}

auto observe_synapse(std::vector<unsigned> ids, std::string _variable) -> void {
  for(unsigned id : ids) {
    observe_synapse(id, _variable);
  }
}

} } // namespace insilico::configuration

#endif
