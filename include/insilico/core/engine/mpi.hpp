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
#include "insilico/core/engine/mpi/generate.hpp"
#include "insilico/core/engine/serial/common.hpp"
#include "insilico/core/engine/mpi/index.hpp"
#include "insilico/core/engine/mpi/value.hpp"

#include <algorithm>
#include <stdio.h>
#include <vector>

namespace insilico { namespace engine {

// global job identity
bool interstate_acquired = false;

auto synchronize_innerstate(state_type &_variables, const double &_time)
    -> void {
  char key[128];
  double updated_value;
  FILE* fptr;
  if(!interstate_acquired) {
    if(!neuron_start_list_ids.empty()) {
      nvar_start_id = neuron_start_list_ids[engine::proc_nsid];
      if(!neuron_end_list_ids.empty()) {
        if(engine::proc_neid >= neuron_start_list_ids.size()) {
          nvar_end_id = neuron_end_list_ids.back();
        }
        else {
          nvar_end_id = neuron_start_list_ids[engine::proc_neid];
        }
      }
    }
    if(!synapse_start_list_ids.empty()) {
      svar_start_id = synapse_start_list_ids[engine::proc_ssid];
      if(!synapse_end_list_ids.empty()) {
        if(engine::proc_seid >= synapse_start_list_ids.size()) {
          svar_end_id = synapse_end_list_ids.back();
        }
        else {
          svar_end_id = synapse_start_list_ids[engine::proc_seid];
        }
      }
    }
    interstate_acquired = true;
  }
  for(unsigned neuron_var_iter = nvar_start_id; neuron_var_iter < nvar_end_id; ++neuron_var_iter) {
    sprintf(key, ".ids/.n%d%s",
            engine::neuron_id_from_index(neuron_var_iter),
            engine::variable_name_from_index(neuron_var_iter).c_str());
    updated_value = _variables[neuron_var_iter];
    fptr = fopen(key, "wb");
    fwrite(&updated_value, sizeof(double), 1, fptr);
    fclose(fptr);
  }
  for(unsigned synapse_var_iter = svar_start_id; synapse_var_iter < svar_end_id; ++synapse_var_iter) {
    sprintf(key, ".ids/.s%d%s",
            engine::synapse_id_from_index(synapse_var_iter),
            engine::variable_name_from_index(synapse_var_iter).c_str());
    updated_value = _variables[synapse_var_iter];
    fptr = fopen(key, "wb");
    fwrite(&updated_value, sizeof(double), 1, fptr);
    fclose(fptr);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

} } // namespace insilico::engine

#endif
