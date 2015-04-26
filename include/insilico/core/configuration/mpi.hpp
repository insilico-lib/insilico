/*
 core/configuration/mpi.hpp - insilico's MPI Configuration source

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

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_MPI_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_MPI_HPP

#include "insilico/core/engine.hpp"
#include "insilico/core/engine/mpi.hpp"
#include "insilico/core/helper/file.hpp"
#include "insilico/core/helper/string.hpp"
#include "insilico/core/mpi.hpp"

#include "insilico/core/configuration/observer.hpp"
#include "insilico/core/configuration/parser.hpp"
#include "insilico/core/configuration/serial.hpp"
#include "insilico/core/injector.hpp"

#include "mpi.h"
#include "stdio.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

namespace insilico { namespace configuration { namespace mpi {

auto finalize() -> void {
  if(insilico::mpi::rank == insilico::mpi::master) {
    remove(".ids/.*");
    rmdir(".ids");
    outstream.close();
    std::cerr << "[insilico::configuration::finalize] "
              << "SUCCESS: Releasing resources."<<'\n';
  }
  MPI_Finalize();
}

auto synchronize_reads() -> void {
  std::string dir_name = ".ids"; // insilico data store directory
  std::string key;
  FILE* fptr;
  mkdir(dir_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  for(auto iterator : engine::value_map) {
    key = ".ids/.";
    key += iterator.first;
    fptr = fopen(key.c_str(), "wb");
    double val = iterator.second;
    fwrite(&val, sizeof(double), 1, fptr);
    fclose(fptr);
  }
}

auto initialize(int argc, char **argv) -> void {
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &insilico::mpi::rank);
  MPI_Comm_size(MPI_COMM_WORLD, &insilico::mpi::size);
  insilico::engine::mpi::assigner_line_master.resize(insilico::mpi::size);
  insilico::engine::mpi::assigner_index_master.resize(insilico::mpi::size);

  if(insilico::mpi::rank == insilico::mpi::master) {
    insilico::configuration::initialize(argc, argv);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  std::vector< unsigned > context_share_size;
  context_share_size.push_back(engine::var_vals.size());
  context_share_size.push_back(engine::value_map.size());
  context_share_size.push_back(engine::index_map.size());

  // Share context sizes
  MPI_Bcast(&context_share_size[0], 3, MPI_UNSIGNED,
            insilico::mpi::master, MPI_COMM_WORLD);
  // Share variables vector
  if(insilico::mpi::rank != insilico::mpi::master) {
    engine::var_vals.resize(context_share_size[0]);
  }
  MPI_Bcast(&engine::var_vals[0], context_share_size[0], MPI_DOUBLE,
            insilico::mpi::master, MPI_COMM_WORLD);

  // Share value unordered_map
  std::vector < std::string > value_map_keys;
  std::vector < double > value_map_values;
  if(insilico::mpi::rank == insilico::mpi::master) {
    for(auto item : engine::value_map) {
      value_map_keys.push_back(item.first);
      value_map_values.push_back(item.second);
    }
  }
  else {
    value_map_keys.resize(context_share_size[1]);
    value_map_values.resize(context_share_size[1]);
  }
  for(unsigned it = 0; it < value_map_keys.size(); ++it) {
    unsigned size_var = value_map_keys[it].length();
    MPI_Bcast(&size_var, 1, MPI_UNSIGNED,
              insilico::mpi::master, MPI_COMM_WORLD);
    char* key_c_string = new char[size_var + 1];
    if(insilico::mpi::rank != insilico::mpi::master) {
      strcpy(key_c_string, value_map_keys[it].c_str());
    }
    MPI_Bcast(&key_c_string[0], size_var, MPI_CHAR,
              insilico::mpi::master, MPI_COMM_WORLD);
    value_map_keys[it].reserve(size_var);
    value_map_keys[it] = key_c_string;
    trim(value_map_keys[it]);
    value_map_keys[it].resize(size_var);
    delete [] key_c_string;
  }
  MPI_Bcast(&value_map_values[0], context_share_size[1], MPI_DOUBLE,
            insilico::mpi::master, MPI_COMM_WORLD);
  if(insilico::mpi::rank != insilico::mpi::master) {
    for(unsigned it = 0; it < value_map_keys.size(); ++it) {
      engine::value_map[value_map_keys[it]] = value_map_values[it];
    }
  }

  // Share index unordered_map
  std::vector < std::string > index_map_keys;
  std::vector < unsigned > index_map_values;
  if(insilico::mpi::rank == insilico::mpi::master) {
    for(auto item : engine::index_map) {
      index_map_keys.push_back(item.first);
      index_map_values.push_back(item.second);
    }
  }
  else {
    index_map_keys.resize(context_share_size[2]);
    index_map_values.resize(context_share_size[2]);
  }
  for(unsigned it = 0; it < index_map_keys.size(); ++it) {
    unsigned size_var = index_map_keys[it].length();
    MPI_Bcast(&size_var, 1, MPI_UNSIGNED,
              insilico::mpi::master, MPI_COMM_WORLD);
    char* key_c_string = new char[size_var + 1];
    if(insilico::mpi::rank == insilico::mpi::master) {
      strcpy(key_c_string, index_map_keys[it].c_str());
    }
    MPI_Bcast(&key_c_string[0], size_var, MPI_CHAR,
              insilico::mpi::master, MPI_COMM_WORLD);
    index_map_keys[it].reserve(size_var);
    index_map_keys[it] = key_c_string;
    trim(index_map_keys[it]);
    index_map_keys[it].resize(size_var);
    delete [] key_c_string;
  }
  MPI_Bcast(&index_map_values[0], context_share_size[2], MPI_UNSIGNED,
            insilico::mpi::master, MPI_COMM_WORLD);
  if(insilico::mpi::rank != insilico::mpi::master) {
    for(unsigned it = 0; it < index_map_keys.size(); ++it) {
      engine::index_map[index_map_keys[it]] = index_map_values[it];
    }
  }

  // Synchronization by master node
  if(insilico::mpi::rank == insilico::mpi::master) {
    synchronize_reads();
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

auto observe_delimiter(const char _delim) -> void {
  if(insilico::mpi::rank == insilico::mpi::master) {
    insilico::configuration::observe_delimiter(_delim);
  }
}

auto observe_header(const bool _flag) -> void {
  if(insilico::mpi::rank == insilico::mpi::master) {
    insilico::configuration::observe_header(_flag);
  }
}

struct observer {
  bool engine_exechook = false;
  auto operator() (state_type &variables, const double t) -> void {
    MPI_Barrier(MPI_COMM_WORLD);
    if(!engine_exechook) {
      engine_exechook = true;
    }
    else {
      engine::mpi::exec_div = false;
      // reseting insilico::mpi::size to new size of no. of computation units
      if(!engine::mpi::rank_resizing &&
         engine::mpi::assigner_line.size() < (unsigned)insilico::mpi::size) {
        insilico::mpi::size = engine::mpi::assigner_line.size();
        if(insilico::mpi::rank == insilico::mpi::master) {
          std::cerr << "[insilico::configuration::mpi::observer] "
                    <<"Simulation is only using " << insilico::mpi::size
                    <<" processes (0 - " << insilico::mpi::size - 1
                    <<").\n";
        }
        engine::mpi::assigner_line_master[insilico::mpi::master]
            .insert(engine::mpi::assigner_line_master[insilico::mpi::master].end(),
                    engine::mpi::assigner_line_master[insilico::mpi::size].begin(),
                    engine::mpi::assigner_line_master[insilico::mpi::size].end());
        engine::mpi::assigner_index_master[insilico::mpi::master]
            .insert(engine::mpi::assigner_index_master[insilico::mpi::master].end(),
                    engine::mpi::assigner_index_master[insilico::mpi::size].begin(),
                    engine::mpi::assigner_index_master[insilico::mpi::size].end());
        engine::mpi::rank_resizing = true;
      }
    }
    engine::mpi::synchronize_innerstate(variables, t);
    if(insilico::mpi::rank == insilico::mpi::master) {
      configuration::write_header_once();
      configuration::outstream << t;
      std::string key;
      double observed_value;
      FILE* fptr;
      for(std::string value_key : configuration::observation_header) {
        key = ".ids/.";
        key += value_key;
        fptr = fopen(key.c_str(), "rb");
        if(fread(&observed_value, sizeof(double), 1, fptr) != 1) {
          configuration::mpi::severe_error();
        }
        fclose(fptr);
        configuration::outstream << configuration::observer_delimiter
                                 << observed_value;
      }
      configuration::outstream << '\n';
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
};

auto observe(std::string _variable) -> void {
  if(insilico::mpi::rank == insilico::mpi::master) {
    configuration::observe(_variable);
  }
}

auto observe_neuron(unsigned _id, std::string _variable) -> void {
  if(insilico::mpi::rank == insilico::mpi::master) {
    configuration::observe_neuron(_id, _variable);
  }
}

auto observe_neuron(std::vector<unsigned> _ids, std::string _variable) -> void {
  if(insilico::mpi::rank == insilico::mpi::master) {
    configuration::observe_neuron(_ids, _variable);
  }
}

auto observe_synapse(unsigned _id, std::string _variable) -> void {
  if(insilico::mpi::rank == insilico::mpi::master) {
    configuration::observe_synapse(_id, _variable);
  }
}

auto observe_synapse(std::vector<unsigned> _ids, std::string _variable) -> void {
  if(insilico::mpi::rank == insilico::mpi::master) {
    configuration::observe_synapse(_ids, _variable);
  }
}

} } } // namespace insilico::configuration::mpi

#endif

