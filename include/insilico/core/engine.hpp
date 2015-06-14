/*
  core/engine.hpp - insilico's Engine primary inclusion header

  Copyright (C) 2014-2015 Pranav Kulkarni, Collins Assisi Lab,
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
 * @file core/engine.hpp
 *
 * Primary header for Engine's file inclusion and common functions.
 */

#pragma once

/**
 * Flag to control the headers for serial or parallel (MPI) related files
 * inclusion.
 */
#ifdef INSILICO_MPI_ENABLE
#include "insilico/core/engine/mpi.hpp"
#else
#include "insilico/core/engine/serial.hpp"
#endif

namespace insilico {
namespace engine {

/**
 * Pre-populates the the list of all pre-Synaptic Neurons connected to
 * a given Neuron against it Neuron ID.
 */
void populate_pre_synaptic_lists() {
  if(!post_neuron.empty()) {
    pre_synaptic_lists.resize( *max_element(post_neuron.begin(), post_neuron.end()) + 1 );
    for(unsigned iterator = 0; iterator < post_neuron.size(); ++iterator) {
      pre_synaptic_lists[ post_neuron[iterator] ].push_back( iterator );
    }
  }
}

} // namespace engine
} // namespace insilico
