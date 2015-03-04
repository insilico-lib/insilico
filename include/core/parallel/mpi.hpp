/*
 parallel/mpi.hpp - insilico's MPI abstraction layer

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

#ifndef INCLUDED_INSILICO_PARALLEL_MPI_HPP
#define INCLUDED_INSILICO_PARALLEL_MPI_HPP

#define INSILICO_MPI_ENABLE
#define MASTER 0

#include "mpi.h"

#include "core/engine.hpp"
#include "parallel/synchronization.hpp"

namespace insilico {

namespace mpi {

int rank, size;

class configuration {
 public:
  static void initialize(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi::rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi::size);
  }

  static void finalize() {    
    MPI_Finalize();
  }
}; // class configuration

void abort() {
  configuration::finalize();
  exit(1);
}

void synchronize(state_type &shared_state) {
  MPI_Barrier(MPI_COMM_WORLD);

  int tag = 3;
  MPI_Request request;

  for(int dest = 0; dest < mpi::size; ++dest) {
    if(dest != mpi::rank) {
      MPI_Isend(&shared_state[0], shared_state.size(), MPI_DOUBLE, dest,
                tag, MPI_COMM_WORLD, &request);
    }
  }

  state_type others_shared_state(shared_state.size());
  MPI_Status status;

  for(int src = 0; src < mpi::size; ++src) {
    if(src != mpi::rank) {
      MPI_Recv(&others_shared_state[0], others_shared_state.size(), MPI_DOUBLE, src,
               tag, MPI_COMM_WORLD, &status);
    }

    for(state_type::size_type iter = 0; iter < shared_state.size(); ++iter) {
      if(shared_state[iter] != others_shared_state[iter]) {
        
      }
    }
  }

}

} // namespace mpi

} // namespace insilico

#endif
