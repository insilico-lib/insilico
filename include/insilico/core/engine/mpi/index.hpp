/*
  core/engine/mpi/index.hpp - engine MPI index management API source

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_MPI_INDEX_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_MPI_INDEX_HPP

#include "insilico/core/type.hpp"
#include "insilico/core/engine/data.hpp"
#include "insilico/core/engine/serial/common.hpp"
#include "insilico/core/engine/mpi/value.hpp"
#include "insilico/core/engine/serial/index.hpp"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico { namespace engine {

auto neuron_index(state_type &_vars, const unsigned &_id,
                  const std::string &_var) -> unsigned {
  unsigned computed_index = neuron_index(_id, _var);

  char compute_key[128];
  std::string key;
  double observed_value = 0;
  FILE* fptr;
  sprintf(compute_key, "n%d%s", _id, _var.c_str());
  key = ".ids/.";
  key += compute_key;
  fptr = fopen(key.c_str(), "rb");
  if(fptr == NULL) {
    std::cerr << "[insilico::engine::neuron_index] Failed to find "<< _var
              << " index for neuron " << _id << ".\n";
    configuration::mpi::severe_error();
  }
  if(fread(&observed_value, sizeof(double), 1, fptr) != 1) {
    std::cerr << "[insilico::engine::neuron_value] Failed to find "<< _var
              << " index for neuron " << _id << ".\n";
    configuration::mpi::severe_error();
  }
  fclose(fptr);

  _vars[computed_index] = observed_value;
  return computed_index;
}

auto synapse_index(state_type &_vars, const unsigned &_id,
                   const std::string &_var) -> unsigned {
  unsigned computed_index = synapse_index(_id, _var);

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

  _vars[computed_index] = observed_value;
  return computed_index;
}

auto neuron_index(state_type &_vars, const unsigned &_id,
                  const std::string &_var, bool& error) -> unsigned {
  unsigned computed_index = neuron_index(_id, _var, error);

  char compute_key[128];
  std::string key;
  double observed_value = 0;
  FILE* fptr;
  sprintf(compute_key, "n%d%s", _id, _var.c_str());
  key = ".ids/.";
  key += compute_key;
  fptr = fopen(key.c_str(), "rb");
  if(error) {
    return 0;
  }
  if(fread(&observed_value, sizeof(double), 1, fptr) != 1) {
    return 0;
  }
  fclose(fptr);

  _vars[computed_index] = observed_value;
  return computed_index;
}

auto synapse_index(state_type &_vars, const unsigned &_id,
                   const std::string &_var, bool& error) -> unsigned {
  unsigned computed_index = synapse_index(_id, _var, error);

  char compute_key[128];
  std::string key;
  double observed_value = 0;
  FILE* fptr;
  sprintf(compute_key, "s%d%s", _id, _var.c_str());
  key = ".ids/.";
  key += compute_key;
  fptr = fopen(key.c_str(), "rb");
  if(error) {
    return 0;
  }
  if(fread(&observed_value, sizeof(double), 1, fptr) != 1) {
    return 0;
  }
  fclose(fptr);

  _vars[computed_index] = observed_value;
  return computed_index;
}

auto get_neuron_indices(state_type &_vars, const std::string &_var)
    -> std::vector<unsigned> {
  std::vector<unsigned> indices;
  bool error = false;
  unsigned idx;
  unsigned total_neurons = neuron_count();
  for(unsigned index = 0; index < total_neurons; ++index) {
    idx = neuron_index(_vars, index, _var, error);
    if(!error) { indices.push_back(idx); }
    error = false;
  }
  return indices;
}

auto get_synapse_indices(state_type &_vars, const std::string &_var)
    -> std::vector<unsigned> {
  std::vector<unsigned> indices;
  bool error = false;
  unsigned idx;
  unsigned total_synapses = synapse_count();
  for(unsigned index = 0; index < total_synapses; ++index) {
    idx = synapse_index(_vars, index, _var, error);
    if(!error) { indices.push_back(idx); }
    error = false;
  }
  return indices;
}

auto get_indices(state_type &_vars, const std::string &_var)
    -> std::vector<unsigned> {
  std::vector<unsigned> indices;
  indices.insert(indices.end(),
                 get_neuron_indices(_vars, _var).begin(),
                 get_neuron_indices(_vars, _var).end());
  indices.insert(indices.end(),
                 get_synapse_indices(_vars, _var).begin(),
                 get_synapse_indices(_vars, _var).end());
  return indices;
}

auto get_pre_neuron_indices(state_type &_vars, const unsigned &_id,
                            const std::string &_var) -> std::vector<unsigned> {
  std::vector<unsigned> indices;
  unsigned idx;
  bool error = false;
  if(!pre_synaptic_lists.empty()) {
    for(unsigned index = 0; index < pre_synaptic_lists[_id].size(); ++index) {
      idx = synapse_index(_vars, pre_synaptic_lists[_id][index],
                          _var, error);
      if(!error) {
        indices.push_back(idx);
      }
    }
  }
  return indices;
}

} } // namespace insilico::engine

#endif
