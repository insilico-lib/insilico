/*
  core/engine/mpi/value.hpp - engine MPI value API source

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_MPI_VALUE_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_MPI_VALUE_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_SERIAL_VALUE_HPP

#include "insilico/core/configuration/mpi/error.hpp"
#include "insilico/core/engine/data.hpp"
#include "insilico/core/type.hpp"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico { namespace engine {

auto neuron_value(const unsigned &_id,
                  const std::string &_var) -> double {
  char compute_key[128];
  std::string key;
  double observed_value = 0;
  FILE* fptr;
  sprintf(compute_key, "n%d%s", _id, _var.c_str());
  key = ".ids/.";
  key += compute_key;
  fptr = fopen(key.c_str(), "rb");
  if(fptr == NULL) {
    std::cerr << "[insilico::engine::neuron_value] Failed to find "<<_var
              << " value for neuron " << _id << ".\n";
    configuration::mpi::severe_error();
  }
  if(fread(&observed_value, sizeof(double), 1, fptr) != 1) {
    std::cerr << "[insilico::engine::neuron_value] Failed to find "<<_var
              << " value for neuron " << _id << ".\n";
    configuration::mpi::severe_error();
  }
  fclose(fptr);
  return observed_value;
}

auto synapse_value(const unsigned &_id,
                   const std::string &_var) -> double {
  char compute_key[128];
  std::string key;
  double observed_value = 0;
  FILE* fptr;
  sprintf(compute_key, "s%d%s", _id, _var.c_str());
  key = ".ids/.";
  key += compute_key;
  fptr = fopen(key.c_str(), "rb");
  if(fptr == NULL) {
    std::cerr << "[insilico::engine::synapse_index] Failed to find "
              << _var << " value for synapse " << _id << ".\n";
    configuration::mpi::severe_error();
  }
  if(fread(&observed_value, sizeof(double), 1, fptr) != 1) {
    std::cerr << "[insilico::engine::synapse_index] Failed to find "
              << _var << " value for synapse " << _id << ".\n";
    configuration::mpi::severe_error();
  }
  fclose(fptr);
  return observed_value;
}

auto neuron_value(const unsigned &_id,
                  const std::string &_var,
                  bool& error) -> double {
  char compute_key[128];
  std::string key;
  double observed_value = 0;
  FILE* fptr;
  sprintf(compute_key, "n%d%s", _id, _var.c_str());
  key = ".ids/.";
  key += compute_key;
  fptr = fopen(key.c_str(), "rb");
  if(fptr == NULL) {
    error = true;
    return observed_value;
  }
  if(fread(&observed_value, sizeof(double), 1, fptr) != 1) {
    error = true;
    return observed_value;
  }
  fclose(fptr);
  return observed_value;
}

auto neuron_value(const unsigned &_id,
                  const std::string &_var,
                  const double &value) -> void {
  char compute_key[128];
  std::string key;
  FILE* fptr;
  sprintf(compute_key, "n%d%s", _id, _var.c_str());
  key = ".ids/.";
  key += compute_key;
  fptr = fopen(key.c_str(), "wb");
  fwrite(&value, sizeof(double), 1, fptr);
  fclose(fptr);
}

auto synapse_value(const unsigned &_id,
                   const std::string &_var,
                   bool& error) -> double {
  char compute_key[128];
  std::string key;
  double observed_value = 0;
  FILE* fptr;
  sprintf(compute_key, "s%d%s", _id, _var.c_str());
  key = ".ids/.";
  key += compute_key;
  fptr = fopen(key.c_str(), "rb");
  if(fptr == NULL) {
    error = true;
    return observed_value;
  }
  if(fread(&observed_value, sizeof(double), 1, fptr) != 1) {
    error = true;
    return observed_value;
  }
  fclose(fptr);
  return observed_value;
}

auto synapse_value(const unsigned &_id,
                   const std::string &_var,
                   const double &value) -> void {
  char compute_key[128];
  std::string key;
  FILE* fptr;
  sprintf(compute_key, "s%d%s", _id, _var.c_str());
  key = ".ids/.";
  key += compute_key;
  fptr = fopen(key.c_str(), "wb");
  fwrite(&value, sizeof(double), 1, fptr);
  fclose(fptr);
}

auto get_neuron_values(const std::string &_var) -> std::vector< double > {
  bool error = false;
  std::vector< double > values;
  unsigned total_neurons = neuron_count();
  for(unsigned index = 0; index < total_neurons; ++index) {
    double value = neuron_value(index, _var, error);
    if(!error) { values.push_back(value); }
    error = false;
  }
  return values;
}

auto get_synapse_values(const std::string &_var) -> std::vector< double > {
  bool error = false;
  std::vector< double > values;
  unsigned total_synapses = synapse_count();
  for(unsigned index = 0; index < total_synapses; ++index) {
    double value = synapse_value(index, _var, error);
    if(!error) { values.push_back(value); }
    error = false;
  }
  return values;
}

auto get_values(const std::string &_var) -> std::vector< double > {
  std::vector< double > values;
  std::vector< double > neuron_values = get_neuron_values(_var);
  std::vector< double > synapse_values = get_synapse_values(_var);
  values.insert(values.end(), neuron_values.begin(), neuron_values.end());
  values.insert(values.end(), synapse_values.begin(), synapse_values.end());
  return values;
}

auto get_pre_neuron_values(const unsigned _id, const std::string &_var)
    -> std::vector<double> {
  std::vector<double> values;
  if(!pre_synaptic_lists.empty()) {
    for(unsigned index = 0; index < pre_synaptic_lists[_id].size(); ++index) {
      values.push_back(synapse_value(pre_synaptic_lists[_id][index], _var));
    }
  }
  return values;
}

auto neuron_value_key(const unsigned _id, const std::string &_var)
    -> decltype(std::string()) {
  char key[128];
  sprintf(key, "n%d%s", _id, _var.c_str());
  if(value_map.find(key) != value_map.end()) { return key; }
  else { return ""; }
}

auto synapse_value_key(const unsigned _id, const std::string &_var)
    -> decltype(std::string()) {
  char key[128];
  sprintf(key, "s%d%s", _id, _var.c_str());
  if(value_map.find(key) != value_map.end()) { return key; }
  else { return ""; }
}

auto get_neuron_value_keys(const std::string &_var)
    -> std::vector< std::string > {
  std::vector< std::string > value_keys;
  std::string key;
  unsigned total_neurons = neuron_count();
  for(unsigned id = 0; id < total_neurons; ++id) {
    key = neuron_value_key(id, _var);
    if(key.compare("") != 0) { value_keys.push_back(key); }
  }
  return value_keys;
}

auto get_synapse_value_keys(const std::string &_var)
    -> std::vector< std::string > {
  std::vector< std::string > value_keys;
  std::string key;
  unsigned total_synapses = synapse_count();
  for(unsigned id = 0; id < total_synapses; ++id) {
    key = synapse_value_key(id, _var);
    if(key.compare("") != 0) { value_keys.push_back(key); }
  }
  return value_keys;
}

auto get_value_keys(const std::string &_var)
    -> std::vector< std::string > {
  std::vector< std::string > value_keys;
  std::vector< std::string > neuron_value_keys;
  std::vector< std::string > synapse_value_keys;
  neuron_value_keys = get_neuron_value_keys(_var);
  synapse_value_keys = get_synapse_value_keys(_var);
  value_keys.insert(value_keys.end(),
                    neuron_value_keys.begin(), neuron_value_keys.end());
  value_keys.insert(value_keys.end(),
                    synapse_value_keys.begin(), synapse_value_keys.end());
  return value_keys;
}

} } // namespace insilico::engine

#endif
