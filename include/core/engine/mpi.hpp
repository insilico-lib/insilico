/*
 core/engine/mpi.hpp - insilico's MPI Engine source

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

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_ENGINE_MPI_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_ENGINE_MPI_HPP

#include "core/mpi.hpp"
#include "core/engine/data.hpp"
#include "core/engine/driver.hpp"
#include "core/engine/generate.hpp"
#include "core/engine/serial/common.hpp"
#include "core/engine/mpi/index.hpp"    // MPI version uses the serial indexing
#include "core/engine/mpi/value.hpp"    // MPI version owns parallel value ver.

#include <algorithm>
#include <stdio.h>
#include <vector>

// for block with indices, assigned to specific rank
#define insilico_mpi_independent_variable_block(x) \
  if(insilico::engine::mpi::block_assigned(x, __LINE__))
#define insilico_mpi_dependent_variable_block(x) \
  if(insilico::engine::mpi::dependent_block_assigned(x, __LINE__))

// for block without indices, assigned to specific rank
#define insilico_mpi_independent_unit \
  if(insilico::engine::mpi::block_assigned(__LINE__))

// for indices without block and not assigned to specific rank
#define insilico_mpi_independent_variable(x) \
  insilico::engine::mpi::register_block(x)
#define insilico_mpi_dependent_variable(x) \
  insilico::engine::mpi::dependent_register_block(x)

// for synchronization among processes
#define insilico_mpi_synchronize(x,t) \
  insilico::engine::mpi::synchronize_innerstate(x,t);

namespace insilico { namespace engine { namespace mpi {

// rank and execution-unit manager
bool exec_div = true;

// global job identity assigner
std::vector< std::vector < unsigned > > assigner;
std::vector< unsigned > assigner_line;
int global_rank = 0;
bool rank_resizing = false;

// update events and indices manager
std::vector< unsigned > update_indices;

auto synchronize_innerstate(state_type &_variables, double _time) -> void {
  std::string key;
  double updated_value;
  FILE* fptr;
  for(unsigned id : engine::mpi::update_indices) {
    for(auto iterator : engine::index_map) {
      if(iterator.second == id) {
        key = ".ids/.";
        key += iterator.first;
        updated_value = _variables[id];
        fptr = fopen(key.c_str(), "wb");
        fwrite(&updated_value, sizeof(double), 1, fptr);
        fclose(fptr);
        break;
      }
    }
  }
  std::vector< std::vector < unsigned > > updated_idxpproc(insilico::mpi::size);
  std::vector< unsigned > update_size(insilico::mpi::size);
  std::vector< unsigned > updated_indices_for_single_process;
  if(insilico::mpi::rank < insilico::mpi::size) {
    updated_idxpproc[insilico::mpi::rank]
        .insert(updated_idxpproc[insilico::mpi::rank].end(),
                update_indices.begin(), update_indices.end());
    update_size[insilico::mpi::rank] = engine::mpi::update_indices.size();
  }
  for(int r = 0; r < insilico::mpi::size; ++r) {
    MPI_Bcast(&update_size[r], 1, MPI_UNSIGNED, r, MPI_COMM_WORLD);
    updated_indices_for_single_process.resize(update_size[r]);
    MPI_Bcast(&updated_indices_for_single_process[0], update_size[r],
              MPI_UNSIGNED, r, MPI_COMM_WORLD);
    for(unsigned index : updated_indices_for_single_process) {
      updated_idxpproc[r].push_back(index);
    }
    for(unsigned id : updated_idxpproc[r]) {
      for(auto iterator : engine::index_map) {
        if(iterator.second == id) {
          key = ".ids/.";
          key += iterator.first;
          fptr = fopen(key.c_str(), "rb");
          if(fread(&updated_value, sizeof(double), 1, fptr) != 1) {
            std::cerr << "[insilico::engine::mpi::synchronize_innerstate]"
                      << " Synchronization failed.\n";
            configuration::mpi::severe_error();
          }
          _variables[id] = updated_value;
          fclose(fptr);
          break;
        }
      }
    }
  }
  engine::mpi::update_indices.clear();
  MPI_Barrier(MPI_COMM_WORLD);
}

auto block_assigned(unsigned _line_id) -> bool {
  if(insilico::mpi::rank < insilico::mpi::size) {
    std::vector< unsigned >::iterator loc_for_line;
    // if rank has been assigned with this line
    loc_for_line = std::find(assigner[insilico::mpi::rank].begin(),
                             assigner[insilico::mpi::rank].end(),
                             _line_id);
    if(loc_for_line != assigner[insilico::mpi::rank].end()) {
      return true;
    }
    // if rank has not occured but line is already there, don't allow
    loc_for_line = std::find(assigner_line.begin(),
                             assigner_line.end(),
                             _line_id);
    if(loc_for_line != assigner_line.end()) {
      return false;
    }
    // line block needs to be assigned to next rank - round robbin
    if((global_rank + 1) >= insilico::mpi::size){
      global_rank = 0;
    }
    else {
      ++global_rank;
    }
    assigner[global_rank].push_back(_line_id);
    assigner_line.push_back(_line_id);
    if(insilico::mpi::rank == global_rank) {
      return true;
    }
  }
  return false;
}

auto block_assigned(unsigned _index, unsigned _line_id) -> bool {
  if(block_assigned(_line_id)) {
    update_indices.push_back(_index);
    return true;
  }
  return false;
}

auto dependent_block_assigned(unsigned _index, unsigned _line_id) -> bool {
  MPI_Barrier(MPI_COMM_WORLD);
  return block_assigned(_index, _line_id);
}

auto register_block(unsigned _index) -> void {
  update_indices.push_back(_index);
}

auto dependent_register_block(unsigned _index) -> void {
  update_indices.push_back(_index);
}

} } } // namespace insilico::engine::mpi 

#endif
