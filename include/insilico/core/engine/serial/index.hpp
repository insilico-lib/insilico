/*
  core/engine/serial/index.hpp - engine's serial index management API

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
 * @file core/engine/serial/index.hpp
 *
 * Functions for accessing indices of simulation variables.
 */

#pragma once

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

namespace insilico {
namespace engine {

/**
 * Finds and returns index of a Neuron variable for
 * a given variable name and Neuron ID.
 *
 * @param local_id Neuron ID.
 * @param variable Name of Neuronal variable to search index for.
 *
 * @return Index of the Neuronal variable in simulation, if successful, else
 *         sends failure message to STDERR.
 */
unsigned neuron_index(unsigned local_id, std::string variable) {
  char key[128];
  sprintf(key, "n%d%s", local_id, variable.c_str());
  if(index_map.find(key) == index_map.end()) {
    std::cerr << "[insilico::engine::neuron_index] Failed to find "<<variable
              <<" index for neuron "<<local_id<<".\n";
    exit(1);
  }
  return index_map[key];
}

/**
 * Finds and returns index of a Synapse variable for
 * a given variable name and Synapse ID.
 *
 * @param local_id Synapse ID.
 * @param variable Name of Synaptic variable to search index for.
 *
 * @return Index of the Synaptic variable in simulation, if successful, else
 *         sends failure message to STDERR.
 */
unsigned synapse_index(unsigned local_id, std::string variable) {
  char key[128];
  sprintf(key, "s%d%s", local_id, variable.c_str());
  if(index_map.find(key) == index_map.end()) {
    std::cerr << "[insilico::engine::synapse_index] Failed to find "<<variable
              <<" index for synapse "<<local_id<<".\n";
    exit(1);
  }
  return index_map[key];
}

/**
 * Finds and returns index of a Neuron variable for
 * a given variable name and Neuron ID.
 *
 * @param local_id Neuron ID.
 * @param variable Name of Neuronal variable to search index for.
 * @param error Ref to bool.
 *
 * @return Index of the Neuronal variable in simulation, if successful, else
 *         sets error to true.
 */
unsigned neuron_index(unsigned local_id, std::string variable, bool& error) {
  char key[128];
  sprintf(key, "n%d%s", local_id, variable.c_str());
  if(index_map.find(key) == index_map.end()) {
    error = true;
    return 0;
  }
  error = false;
  return index_map[key];
}

/**
 * Finds and returns index of a Synapse variable for
 * a given variable name and Synapse ID.
 *
 * @param local_id Synapse ID.
 * @param variable Name of Synaptic variable to search index for.
 * @param error Ref to bool.
 *
 * @return Index of the Synaptic variable in simulation, if successful, else
 *         sets error to true.
 */
unsigned synapse_index(unsigned local_id, std::string variable, bool& error) {
  char key[128];
  sprintf(key, "s%d%s", local_id, variable.c_str());
  if(index_map.find(key) == index_map.end()) {
    error = true;
    return 0;
  }
  error = false;
  return index_map[key];
}

/**
 * Finds and returns indices of all occurances of
 * Neuronal variable with a given Neuronal variable name.
 *
 * @param variable Name of Neuronal variable to search indices for.
 *
 * @return indices of all found occurances of Neuronal variable.
 */
std::vector<unsigned> get_neuron_indices(std::string variable) {
  std::vector<unsigned> indices;
  bool error = false;
  unsigned idx;
  unsigned total_neurons = neuron_count();
  for(unsigned index = 0; index < total_neurons; ++index) {
    idx = neuron_index(index, variable, error);
    if(!error) { indices.push_back(idx); }
    error = false;
  }
  return indices;
}

/**
 * Finds and returns indices of all occurances of
 * Synaptic variable with a given Synaptic variable name.
 *
 * @param variable Name of Synaptic variable to search indices for.
 *
 * @return indices of all found occurances of Synaptic variable.
 */
std::vector<unsigned> get_synapse_indices(std::string variable) {
  std::vector<unsigned> indices;
  bool error = false;
  unsigned idx;
  unsigned total_synapses = synapse_count();
  for(unsigned index = 0; index < total_synapses; ++index) {
    idx = synapse_index(index, variable, error);
    if(!error) { indices.push_back(idx); }
    error = false;
  }
  return indices;
}

/**
 * Finds and returns indices of all occurances of Neuronal and Synaptic
 * variable with given variable name.
 *
 * @param variable Name of variable to search indices for.
 *
 * @return combined indices of all found occurances of Neuronal and
 *         Synaptic variable.
 */
std::vector<unsigned> get_indices(std::string variable) {
  std::vector<unsigned> indices;
  auto neuron_indices = get_neuron_indices(variable);
  auto synapse_indices = get_synapse_indices(variable);
  indices.insert(indices.end(), neuron_indices.begin(), neuron_indices.end());
  indices.insert(indices.end(), synapse_indices.begin(), synapse_indices.end());
  return indices;
}

/**
 * Finds and returns Neuron ID for given Neuron variable index.
 *
 * @param index Neuron variable index.
 *
 * @return unsigned Neuron ID or dislay error message on STDERR.
 */
unsigned neuron_id_from_index(unsigned index) {
  if(prepopulated_neuron_ids.empty() ||
     index < neuron_start_list_ids.front() ||
     index > neuron_end_list_ids.back())  {
    std::cerr << "[insilico::engine::neuron_id_from_index] "
              << "Failed to find index "<< index << std::endl;
    exit(1);
  }
  return prepopulated_neuron_ids[index - neuron_start_list_ids.front()];
}

/**
 * Finds and returns Neuron ID for given Neuron variable index.
 *
 * @param index Neuron variable index.
 * @param error Ref to bool.
 *
 * @return unsigned Neuron ID, if successful, else set error to true.
 */
unsigned neuron_id_from_index(unsigned index, bool &error) {
  if(prepopulated_neuron_ids.empty() ||
     index < neuron_start_list_ids.front() ||
     index >= neuron_end_list_ids.back())  {
    error = true;
    return 0;
  }
  error = false;
  return prepopulated_neuron_ids[index - neuron_start_list_ids.front()];
}

/**
 * Finds and returns Synapse ID for given Synaptic variable index.
 *
 * @param index Synaptic variable index.
 *
 * @return unsigned Synapse ID or dislay error message on STDERR.
 */
unsigned synapse_id_from_index(unsigned index) {
  if(prepopulated_synapse_ids.empty() ||
     index < synapse_start_list_ids.front() ||
     index > synapse_end_list_ids.back())  {
    std::cerr << "[insilico::engine::synapse_id_from_index] "
              << "Failed to find index "<< index << std::endl;
    exit(1);
  }
  return prepopulated_synapse_ids[index - synapse_start_list_ids.front()];
}

/**
 * Finds and returns Synapse ID for given Synaptic variable index.
 *
 * @param index Synaptic variable index.
 * @param error Ref to bool.
 *
 * @return unsigned Synaptic ID, if successful, else set error to true.
 */
unsigned synapse_id_from_index(unsigned index, bool &error) {
  if(prepopulated_synapse_ids.empty() ||
     index < synapse_start_list_ids.front() ||
     index >= synapse_end_list_ids.back())  {
    error = true;
    return 0;
  }
  error = false;
  return prepopulated_synapse_ids[index - synapse_start_list_ids.front()];
}

std::string variable_name_from_index(unsigned index) {
  if(index < var_list_ids.size()) {
    return var_list_ids[index];
  }
  return "";
}

/**
 *
 */
std::vector<unsigned> get_pre_neuron_indices(unsigned local_id, std::string variable) {
  std::vector<unsigned> indices;
  unsigned idx;
  bool error = false;
  if(!pre_synaptic_lists.empty()) {
    for(unsigned index = 0; index < pre_synaptic_lists[local_id].size(); ++index) {
      idx = synapse_index(pre_synaptic_lists[local_id][index], variable, error);
      if(!error) {
        indices.push_back(idx);
      }
    }
  }
  return indices;
}

} // namespace engine
} // namespace insilico
