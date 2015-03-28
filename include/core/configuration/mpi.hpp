/*
 core/configuration/mpi.hpp - insilico's MPI Configuration source

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

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_MPI_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_MPI_HPP

#include "core/engine.hpp"
#include "core/engine/mpi.hpp"
#include "core/helper/file.hpp"
#include "core/helper/string.hpp"
#include "core/mpi.hpp"

#include "core/configuration/observer.hpp"
#include "core/configuration/parser.hpp"
#include "core/configuration/serial.hpp"
#include "core/injector.hpp"

#include "dir.h"
#include "mpi.h"
#include "process.h"
#include "stdio.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico { namespace configuration { namespace mpi {

auto synchronize_reads() -> void {
  std::string dir_name = ".ids"; // insilico data store directory
  std::string key;
  FILE* fptr;
  if(!mkdir(dir_name.c_str())) {
    std::cerr << "[insilico::configuration::mpi::synchronize_reads] insilico Data Store \
                  not available.\nAborting Simulation.\n";
    insilico::configuration::finalize();
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  for(auto iterator = engine::value_map.cbegin(); iterator != engine::value_map.cend(); ++iterator) {
    key = ".ids/.";
    key += iterator->first;
    fptr = fopen(key, "wb");
    fwrite(iterator->second, 1, sizeof(double), fptr);
    fclose();
  }
}

auto synchronize(state_type &_variables, const double _t) -> void {
  std::unordered_map<std::string, unsigned>::const_iterator iterator;
  std::string key;
  double updated_value;
  FILE* fptr;
  for(unsigned id : engine::mpi::update_indices) {
    iterator = engine::index_map.find(id);
    key = ".ids/.";
    key += iterator->first;
    updated_value = _variables[id];
    fptr = fopen(key, "rb");
    fwrite(&updated_value, 1, sizeof(double), fptr);
    fclose();
  }
  engine::mpi::update_indices.clear();
  MPI_Barrier(MPI_COMM_WORLD);
}

auto initialize(int argc, char **argv) -> auto {
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi::rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi::size);
  if(mpi::rank == mpi::master) {
    insilico::configuration::initialize(argc, argv);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  // Share variables vector
  unsigned long variables_size = engine::var_vals.size();
  MPI_Bcast(&variables_size, 1, MPI_UNSIGNED_LONG, mpi::master, MPI_COMM_WORLD);
  if(mpi::rank != mpi::master) {
    engine::var_vals.resize(variables_size);
  }
  MPI_Bcast(&engine::var_vals[0], variables_size, MPI_DOUBLE, mpi::master, MPI_COMM_WORLD);

  // Share index unordered_map
  unsigned long shared_indices_size = engine::index_map.size();
  std::vector < std::string > index_map_keys;
  std::vector < unsigned > index_map_values;
  MPI_Bcast(&shared_indices_size, 1, MPI_UNSIGNED_LONG, mpi::master, MPI_COMM_WORLD);
  if(mpi::rank == mpi::master) {
    for(auto iterator = engine::index_map.cbegin(); iterator != engine::index_map.cend(); ++iterator) {
      index_map_keys.push_back(iterator->first);
      index_map_values.push_back(iterator->second);
    }
  }
  else {
    index_map_keys.resize(shared_indices_size);
    index_map_values.resize(shared_indices_size);
  }
  for(unsigned it = 0; it < index_map_keys.size(); ++it) {
    char* key_c_string = index_map_keys[it].c_str();
    MPI_Bcast(&key_c_string[0], index_map_keys[it].size(), MPI_CHAR, mpi::master, MPI_COMM_WORLD);
    index_map_keys[it] = key_c_string;
  }
  MPI_Bcast(&index_map_values[0], shared_indices_size, MPI_UNSIGNED_LONG, mpi::master, MPI_COMM_WORLD);
  if(mpi::rank != mpi::master) {
    for(unsigned it = 0; it < index_map_keys.size(); ++it) {
      engine::index_map[index_map_keys[it]] = index_map_values[it];
    }
  }

  // Share value unordered_map
  unsigned long shared_values_size = engine::value_map.size();
  std::vector < std::string > value_map_keys;
  std::vector < double > value_map_values;
  MPI_Bcast(&shared_values_size, 1, MPI_UNSIGNED_LONG, mpi::master, MPI_COMM_WORLD);
  if(mpi::rank == mpi::master) {
    for(auto iterator = engine::value_map.cbegin(); iterator != engine::value_map.cend(); ++iterator) {
      value_map_keys.push_back(iterator->first);
      value_map_values.push_back(iterator->second);
    }
  }
  else {
    value_map_keys.resize(shared_values_size);
    value_map_values.resize(shared_values_size);
  }
  for(unsigned it = 0; it < value_map_keys.size(); ++it) {
    char* key_c_string = value_map_keys[it].c_str();
    MPI_Bcast(&key_c_string[0], value_map_keys[it].size(), MPI_CHAR, mpi::master, MPI_COMM_WORLD);
    value_map_keys[it] = key_c_string;
  }
  MPI_Bcast(&value_map_values[0], shared_values_size, MPI_DOUBLE, mpi::master, MPI_COMM_WORLD);
  if(mpi::rank != mpi::master) {
    for(unsigned it = 0; it < value_map_keys.size(); ++it) {
      engine::value_map[value_map_keys[it]] = value_map_values[it];
    }
  }

  // Synchronization by Master node
  if(mpi::rank == mpi::master) {
    synchronize_reads();
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

auto finalize() -> auto {
  if(mpi::rank == mpi::master) {
    insilico::configuration::finalize();
  }
  MPI_Finalize();
}

auto observe_delimiter(const char _delim) -> void {
  if(mpi::rank == mpi::master) {
    insilico::configuration::observe_delimiter(_delim);
  }
}

auto observe_header(const bool _flag) -> void {
  if(mpi::rank == mpi::master) {
    insilico::configuration::observe_header(_flag);
  }
}

struct observer {
  auto operator() (state_type &variables, const double t) -> void {
    synchronize(variables, t);
    if(mpi::rank == mpi::master) {
      insilico::configuration::write_header_once();
      insilico::configuration::outstream << t;
      std::string key;
      double observed_value;
      FILE* fptr;
      for(std::string value_key : insilico::configuration::observation_header) {
        key = ".ids/.";
        key += value_key;
        fptr = fopen(key, "rb");
        fread(&observed_value, 1, sizeof(double), fptr);
        fclose();
        insilico::configuration::outstream << insilico::configuration::observer_delimiter << observed_value;
      }
      insilico::configuration::outstream << '\n';
    }
  }
};

auto observe(std::string _variable) -> void {
  if(mpi::rank == mpi::master) {
    insilico::configuration::observe(_variable);
  }
}

auto observe_neuron(unsigned _id, std::string _variable) -> void {
  if(mpi::rank == mpi::master) {
    insilico::configuration::observe_neuron(_id, _variable);
  }
}

auto observe_neuron(std::vector< unsigned > _ids, std::string _variable) -> void {
  if(mpi::rank == mpi::master) {
    insilico::configuration::observe_neuron(_ids, _variable);
  }
}

auto observe_synapse(unsigned _id, std::string _variable) -> void {
  if(mpi::rank == mpi::master) {
    insilico::configuration::observe_synapse(_id, _variable);
  }
}

auto observe_synapse(std::vector< unsigned > _ids, std::string _variable) -> void {
  if(mpi::rank == mpi::master) {
    insilico::configuration::observe_synapse(_ids, _variable);
  }
}

auto severe_error() -> void {
  std::cerr << "***...SIMULATION FAILED...***\n";
  MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
}

} } } // namespace insilico::configuration::mpi

#endif

