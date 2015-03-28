/*
 core/engine/mpi.hpp - insilico's MPI Engine source

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

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_ENGINE_MPI_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_ENGINE_MPI_HPP

#include "core/mpi.hpp"
#include "core/engine/serial/index.hpp" // MPI version uses the serial indexing
#include "core/engine/mpi/value.hpp"    // MPI version has its own parallel value version

#include <vector>

namespace insilico { namespace engine { namespace mpi {

// Rank manager
unsigned global_rank = 0;

// Event update
std::vector< unsigned > update_indices;

auto mpi_work_assigned() -> bool {
  if(_rank == global_rank) {
    if(global_rank < mpi::size) { ++global_rank; }
    else { global_rank = 0; }
    return true;
  }
  return false;
}

#define INSILICO_MPI_REGISTER   if(mpi_register_update())
auto mpi_register_update() -> bool {
  return mpi_work_assignment();
}

#define INSILICO_MPI_PROCESS(x) if(mpi_process_update(x))
auto mpi_process_update(unsigned _index) -> bool {
  updated_value.push_back(_index);
  return mpi_work_assignment();
}

} } } // namespace insilico::engine::mpi 

#endif

