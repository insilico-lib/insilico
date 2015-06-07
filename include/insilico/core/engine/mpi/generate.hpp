/*
  core/engine/mpi/generate.hpp - Parallel type generation and execution

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_MPI_GENERATE_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_MPI_GENERATE_HPP

#include "insilico/core/type.hpp"
#include "insilico/core/engine/driver.hpp"
#include "mpi.h"

#include <boost/numeric/odeint.hpp>

#include <cmath>
#include <iostream>
#include <vector>

namespace insilico { namespace engine {

std::vector< Neuron* > neuron_objects;
std::vector< Synapse* > synapse_objects;
std::vector< unsigned > neuron_objects_count, synapse_objects_count;
unsigned total_neuron_objects = 0, total_synapse_objects = 0;
unsigned proc_nsid, proc_neid, proc_ssid, proc_seid;
unsigned nvar_start_id, nvar_end_id, svar_start_id, svar_end_id;
bool proc_set = false, n_act = false, s_act = false;

template<class T>
auto generate_neuron(unsigned count = 1) -> void {
  neuron_objects.push_back(new T());
  neuron_objects_count.push_back(count);
  total_neuron_objects += count;
}

template<class T>
auto generate_synapse(unsigned count = 1) -> void {
  synapse_objects.push_back(new T());
  synapse_objects_count.push_back(count);
  total_synapse_objects += count;
}

auto work_per_process() -> void {
  if(!proc_set) {
    unsigned srawid, erawid;
    char srawidc, erawidc;    
    if(insilico::mpi::rank == insilico::mpi::master) {
      unsigned total_components = total_neuron_objects + total_synapse_objects;
      unsigned slotsz = std::ceil(total_components / insilico::mpi::size);
      std::vector<unsigned> ids;
      std::vector<char> idsc;
      unsigned i;
      for(i = 0; i < total_neuron_objects; ++i) {
        ids.push_back(i);
        idsc.push_back('n');
      }
      ids.push_back(i);
      idsc.push_back('n');
      for(i = 0; i < total_synapse_objects; ++i) {
        ids.push_back(i);
        idsc.push_back('s');
      }      
      ids.push_back(i);
      idsc.push_back('s');
      for(unsigned rank = 1; rank < insilico::mpi::size; ++rank) {
        MPI_Send(&ids[rank*slotsz], 1, MPI_UNSIGNED, rank, 0, MPI_COMM_WORLD);
        MPI_Send(&ids[(rank+1)*slotsz], 1, MPI_UNSIGNED, rank, 0, MPI_COMM_WORLD);
        MPI_Send(&idsc[rank*slotsz], 1, MPI_CHAR, rank, 0, MPI_COMM_WORLD);
        MPI_Send(&idsc[(rank+1)*slotsz], 1, MPI_CHAR, rank, 0, MPI_COMM_WORLD);
      }
      srawid = ids[insilico::mpi::rank*slotsz];
      erawid = ids[(insilico::mpi::rank+1)*slotsz];
      srawidc = idsc[insilico::mpi::rank*slotsz];
      erawidc = idsc[(insilico::mpi::rank+1)*slotsz];
    }
    else {
      MPI_Recv(&srawid, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&erawid, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&srawidc, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&erawidc, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    switch(srawidc) {
      case 'n':
        proc_nsid = srawid;
        n_act = true;
        break;
      case 's':
        proc_ssid = srawid;
        s_act = true;
        break;
      default:
        std::cerr << "[insilico::engine::driver] Rank : " << insilico::mpi::rank
                  << " neuron and synapse start distribution failed.\n";
    }
    switch(erawidc) {
      case 'n':
        if(n_act == true) {
          proc_neid = erawid;
        }        
        break;
      case 's':
        if(n_act == true) {
          proc_neid = total_neuron_objects;
          proc_ssid = 0;
          proc_seid = srawid;
        }
        else if(s_act == true) {
          proc_seid = srawid;
        }
        break;
      default:
        std::cerr << "[insilico::engine::driver] Rank : " << insilico::mpi::rank
                  << " neuron and synapse end distribution failed.\n";
    }
    proc_set = true; 
  }
}

auto driver::operator()(state_type &_state, state_type &_dxdt, const double _t) -> void {
  work_per_process(); // processes only once
  unsigned ultimate_count = 0;
  for(std::vector<Neuron*>::size_type type = 0; type < neuron_objects.size(); ++type) {
    for(unsigned iter = 0; iter < neuron_objects_count[type]; ++iter, ++ultimate_count) {
      if(ultimate_count >= proc_nsid && ultimate_count < proc_neid) {
        neuron_objects[type] -> ode_set(_state, _dxdt, _t, ultimate_count);
      }
    }
  }
  ultimate_count = 0;
  for(std::vector<Synapse*>::size_type type = 0; type < synapse_objects.size(); ++type) {
    for(unsigned iter = 0; iter < synapse_objects_count[type]; ++iter, ++ultimate_count) {
      if(ultimate_count >= proc_ssid && ultimate_count < proc_seid) {
        synapse_objects[type] -> ode_set(_state, _dxdt, _t, ultimate_count);
      }
    }
  }
}

} } // namespace insilico::engine

#endif
