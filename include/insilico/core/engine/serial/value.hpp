/*
  core/engine/serial/value.hpp - insilico's Engine value fetch and store API

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
/**
 * @file core/engine/serial/value.hpp
 *
 * Functions for creating and updating as well as retrieving values of
 * simulation variables and parameters.
 */

#pragma once

#include "insilico/core/configuration/error.hpp"
#include "insilico/core/engine/data.hpp"
#include "insilico/core/engine/serial/common.hpp"
#include "insilico/core/type.hpp"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico {
namespace engine {

/**
 * Checks if the value for a given key is present or not
 * in value map in engine's data store.
 *
 * @param key C string with name of key.
 *
 * @return true if key is present or false otherwise.
 */
bool value_key_present(char* key) {
  return (value_map.find(key) != value_map.end());
}

/**
 * Finds and returns the value of Neuronal variable
 * with given Neuron ID and name of the variable.
 *
 * @param local_id Neuron ID.
 * @param variable_name name of Neuronal variable.
 *
 * @return value of Neuronal variable if variable is present,
 *         else print the error message on STDERR and exit.
 */
double neuron_value(unsigned local_id, std::string variable_name) {
  char key[128];
  sprintf(key, "n%d%s", local_id, variable_name.c_str());
  if(!value_key_present(key)) {
    std::cerr << "[insilico::engine::neuron_value] Failed to find "<<variable_name
              << " value for neuron " << local_id << ".\n";
    configuration::severe_error();
  }
  return value_map[key];
}

/**
 * Finds and returns the value of Synaptic variable
 * with given Synapse ID and name of the variable.
 *
 * @param local_id Synapse ID.
 * @param variable_name name of Synapse variable.
 *
 * @return value of Synaptic variable if variable is present,
 *         else print the error message on STDERR and exit.
 */
double synapse_value(unsigned local_id, std::string variable_name) {
  char key[128];
  sprintf(key, "s%d%s", local_id, variable_name.c_str());
  if(!value_key_present(key)) {
    std::cerr << "[insilico::engine::synapse_index] Failed to find "
              << variable_name << " value for synapse " << local_id << ".\n";
    configuration::severe_error();
  }
  return value_map.at(key);
}

/**
 * Finds and returns the value of Neuronal variable
 * with given Neuron ID and name of the variable.
 *
 * @param local_id Neuron ID.
 * @param variable_name name of Neuronal variable.
 * @param error Reference to bool.
 *
 * @return value of Neuronal variable if variable is present,
 *         else set "error" to true.
 */
double neuron_value(unsigned local_id, std::string variable_name, bool& error) {
  char key[128];
  sprintf(key, "n%d%s", local_id, variable_name.c_str());
  if(!value_key_present(key)) {
    error = true;
    return 0;
  }
  error = false;
  return value_map[key];
}

/**
 * Creates new Neuronal variable or updates the existing Neuronal variable
 * belonging to specific Neuron ID, with specific variable name and value.
 *
 * @param local_id Neuron ID.
 * @param variable_name name of Neuronal variable.
 * @param value new or updated value for variable.
 */
void neuron_value(unsigned local_id, std::string variable_name, double value) {
  char key[128];
  sprintf(key, "n%d%s", local_id, variable_name.c_str());
  value_map[key] = value;
}

/**
 * Finds and returns the value of Synaptic variable
 * with given Synapse ID and name of the variable.
 *
 * @param local_id Synapse ID.
 * @param variable_name name of Synapse variable.
 * @param error Reference to bool.
 *
 * @return value of Synaptic variable if variable is present,
 *         else set "error" to true.
 */
double synapse_value(unsigned local_id, std::string variable_name, bool& error) {
  char key[128];
  sprintf(key, "s%d%s", local_id, variable_name.c_str());
  if(!value_key_present(key)) {
    error = true;
    return 0;
  }
  error = false;
  return value_map[key];
}

/**
 * Creates new Synaptic variable or updates the existing Synaptic variable
 * belonging to specific Synapse ID, with specific variable name and value.
 *
 * @param local_id Synapse ID.
 * @param variable_name name of Synaptic variable.
 * @param value new or updated value for variable.
 */
void synapse_value(unsigned local_id, std::string variable_name, double value) {
  char key[128];
  sprintf(key, "s%d%s", local_id, variable_name.c_str());
  value_map[key] = value;
}

/**
 * Finds and returns the collection of values for all Neuronal variables
 * with given variable name.
 *
 * @param variable_name name of Neuronal variable.
 *
 * @return collection of values in std::vector<double>.
 */
std::vector<double> get_neuron_values(std::string variable_name) {
  bool error = false;
  std::vector< double > values;
  unsigned total_neurons = neuron_count();
  for(unsigned index = 0; index < total_neurons; ++index) {
    double value = neuron_value(index, variable_name, error);
    if(!error) { values.push_back(value); }
    error = false;
  }
  return values;
}

/**
 * Finds and returns the collection of values for all Synaptic variables
 * with given variable name.
 *
 * @param variable_name name of Synaptic variable.
 *
 * @return collection of values in std::vector<double>.
 */
std::vector<double> get_synapse_values(std::string variable_name) {
  bool error = false;
  std::vector< double > values;
  unsigned total_synapses = synapse_count();
  for(unsigned index = 0; index < total_synapses; ++index) {
    double value = synapse_value(index, variable_name, error);
    if(!error) { values.push_back(value); }
    error = false;
  }
  return values;
}

/**
 * Finds and returns the collection of values for all variables
 * with given variable name irrespective of being part of
 * Neuron or Synapse.
 *
 * @param variable_name name of Neuronal or Synaptic variable.
 *
 * @return collection of values in std::vector<double>.
 */
std::vector<double> get_values(std::string variable_name) {
  std::vector< double > values;
  std::vector< double > neuron_values = get_neuron_values(variable_name);
  std::vector< double > synapse_values = get_synapse_values(variable_name);
  values.insert(values.end(), neuron_values.begin(), neuron_values.end());
  values.insert(values.end(), synapse_values.begin(), synapse_values.end());
  return values;
}

/**
 * Finds and returns the collection of values for variables that belongs
 * to Synapnes connected to the Neuron with given ID and has given variable name.
 *
 * @param local_id Neuron ID.
 * @param variable_name name of Synaptic variable.
 *
 * @return collection of current values for all Synaptic variable matching name
 *         "variable_name" and are part of pre-synaptic terminals to Neuron
 *         with ID "local_id", if successful, else returns empty collection.
 */
std::vector<double> get_pre_neuron_values(unsigned local_id, std::string variable_name) {
  std::vector<double> values;
  double value;
  bool error = false;
  if(!pre_synaptic_lists.empty()) {
    for(unsigned index = 0; index < pre_synaptic_lists[local_id].size(); ++index) {
      value = synapse_value(pre_synaptic_lists[local_id][index], variable_name, error);
      if(!error) {
        values.push_back(value);
      }
    }
  }
  return values;
}

/**
 * Finds and returns the internal representation key for Neuronal variable.
 *
 * @param local_id Neuron ID.
 * @param variable_name name of Neuronal variable.
 *
 * @return key as std::string, if successful, else empty string.
 */
std::string neuron_value_key(unsigned local_id, std::string variable_name) {
  char key[128];
  sprintf(key, "n%d%s", local_id, variable_name.c_str());
  if(value_map.find(key) != value_map.end()) {
    return key;
  }
  else {
    return "";
  }
}

/**
 * Finds and returns the internal representation key for Synaptic variable.
 *
 * @param local_id Synapse ID.
 * @param variable_name name of Synaptic variable.
 *
 * @return key as std::string, if successful, else empty string.
 */
std::string synapse_value_key(unsigned local_id, std::string variable_name) {
  char key[128];
  sprintf(key, "s%d%s", local_id, variable_name.c_str());
  if(value_map.find(key) != value_map.end()) {
    return key;
  }
  else {
    return "";
  }
}

/**
 * Finds and returns the collection of internal representation key
 * for all matching Neuronal variable names.
 *
 * @param variable_name name of Neuronal variable.
 *
 * @return key as std::vector of std::string's, if successful,
 *         else empty std::vector.
 */
std::vector<std::string> get_neuron_value_keys(std::string variable_name) {
  std::vector<std::string> value_keys;
  std::string key;
  unsigned total_neurons = neuron_count();
  for(unsigned id = 0; id < total_neurons; ++id) {
    key = neuron_value_key(id, variable_name);
    if(key.compare("") != 0) {
      value_keys.push_back(key);
    }
  }
  return value_keys;
}

/**
 * Finds and returns the collection of internal representation key
 * for all matching Synaptic variable names.
 *
 * @param variable_name name of Synaptic variable.
 *
 * @return key as std::vector of std::string's, if successful,
 *         else empty std::vector.
 */
std::vector<std::string> get_synapse_value_keys(std::string variable_name) {
  std::vector<std::string> value_keys;
  std::string key;
  unsigned total_synapses = synapse_count();
  for(unsigned id = 0; id < total_synapses; ++id) {
    key = synapse_value_key(id, variable_name);
    if(key.compare("") != 0) {
      value_keys.push_back(key);
    }
  }
  return value_keys;
}

/**
 * Finds and returns the collection of internal representation key
 * for all matching variable names irrespective of being related to
 * Neurons or Synapses.
 *
 * @param variable_name name of Neuronal or Synaptic variable.
 *
 * @return key as std::vector of std::string's, if successful,
 *         else empty std::vector.
 */
std::vector<std::string> get_value_keys(std::string variable_name) {
  std::vector<std::string> value_keys;
  std::vector<std::string> neuron_value_keys;
  std::vector<std::string> synapse_value_keys;
  neuron_value_keys = get_neuron_value_keys(variable_name);
  synapse_value_keys = get_synapse_value_keys(variable_name);
  value_keys.insert(value_keys.end(), neuron_value_keys.begin(),
                    neuron_value_keys.end());
  value_keys.insert(value_keys.end(), synapse_value_keys.begin(),
                    synapse_value_keys.end());
  return value_keys;
}

} // namespace engine
} // namespace insilico
