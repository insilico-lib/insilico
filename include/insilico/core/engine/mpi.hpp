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

#include "insilico/core/mpi.hpp"
#include "insilico/core/engine/data.hpp"
#include "insilico/core/engine/driver.hpp"
#include "insilico/core/engine/generate.hpp"
#include "insilico/core/engine/serial/common.hpp"
#include "insilico/core/engine/mpi/index.hpp"    // MPI version uses the serial indexing
#include "insilico/core/engine/mpi/value.hpp"    // MPI version owns parallel value ver.

#include <algorithm>
#include <stdio.h>
#include <vector>

// for block with indices, assigned to specific rank
#define insilico_mpi_independent_variable_block(id, x) \
  if(insilico::engine::mpi::block_assigned(id, x, __LINE__))
#define insilico_mpi_dependent_variable_block(id, x) \
  if(insilico::engine::mpi::dependent_block_assigned(id, x, __LINE__))

// for block without indices, assigned to specific rank
#define insilico_mpi_independent_unit(x) \
  if(insilico::engine::mpi::block_assigned(x, __LINE__))

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
std::vector<std::vector<unsigned>> assigner_line_master;
std::vector<std::vector<unsigned>> assigner_index_master;
std::vector< unsigned > assigner_line;
std::vector< unsigned > assigner_index;
int global_rank = 0;
bool rank_resizing = false;

// update events and indices manager
std::vector< unsigned > update_indices;

auto synchronize_innerstate(state_type &_variables, const double &_time)
    -> void {
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
  engine::mpi::update_indices.clear();
  MPI_Barrier(MPI_COMM_WORLD);
}

auto block_assigned(const unsigned &_index,
                    const unsigned &_line_id) -> bool {
  if(insilico::mpi::rank < insilico::mpi::size) {
    std::vector< unsigned >::iterator loc_for_line, loc_for_index;
    // if this rank has been assigned with this line-index pair
    loc_for_line = std::find(assigner_line_master[insilico::mpi::rank].begin(),
                             assigner_line_master[insilico::mpi::rank].end(),
                             _line_id);
    loc_for_index = std::find(assigner_index_master[insilico::mpi::rank].begin(),
                              assigner_index_master[insilico::mpi::rank].end(),
                              _index);
    if(loc_for_line != assigner_line_master[insilico::mpi::rank].end() &&
       loc_for_line != assigner_index_master[insilico::mpi::rank].end()) {
      return true; // yes, this line-index pair is assigned to this rank!
    }
    // if line-index pair is already assigned to some other process,
    // don't allow this process
    loc_for_line = std::find(assigner_line.begin(),
                             assigner_line.end(),
                             _line_id);
    loc_for_index = std::find(assigner_index.begin(),
                             assigner_index.end(),
                              _index);
    if(loc_for_line != assigner_line.end() &&
       loc_for_index != assigner_index.end()) {
      return false;
    }
    // line block needs to be assigned to next rank - round robbin
    if((global_rank + 1) >= insilico::mpi::size){
      global_rank = 0;
    }
    else {
      ++global_rank;
    }
    assigner_line_master[global_rank].push_back(_line_id);
    assigner_index_master[global_rank].push_back(_index);
    assigner_line.push_back(_line_id);
    assigner_index.push_back(_index);
    if(insilico::mpi::rank == global_rank) {
      return true;
    }
  }
  return false;
}

auto block_assigned(const unsigned &_index,
                    const unsigned &_var_index,
                    const unsigned &_line_id) -> bool {
  if(block_assigned(_index, _line_id)) {
    update_indices.push_back(_var_index);
    return true;
  }
  return false;
}

auto dependent_block_assigned(const unsigned &_index,
                              const unsigned &_var_index,
                              const unsigned &_line_id) -> bool {
  MPI_Barrier(MPI_COMM_WORLD);
  return block_assigned(_index, _var_index, _line_id);
}

auto register_block(const unsigned &_index) -> void {
  update_indices.push_back(_index);
}

auto dependent_register_block(const unsigned &_index) -> void {
  update_indices.push_back(_index);
}

} } } // namespace insilico::engine::mpi 

#endif
