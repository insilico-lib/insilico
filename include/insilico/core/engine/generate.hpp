/*
  core/engine/generate.hpp - Type generation and management

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
/**
 * @file core/engine/generate.hpp
 *
 * Uses generic containers for Neuron and Synapse types,
 * object generation happens at run-time with
 * non-object specific variable integration calls.
 */

#pragma once

#include "insilico/core/type.hpp"
#include "insilico/core/engine/driver.hpp"

#include <boost/numeric/odeint.hpp>

#include <iostream>
#include <vector>

namespace insilico {
namespace engine {

/**
 * Generic store for all objects under Neuron types.
 */
std::vector<Neuron*> neuron_objects;

/**
 * Maintains the total count of object of Neuron types.
 */
std::vector<unsigned> neuron_objects_count;

/**
 * Generic store for all objects under Synapse types.
 */
std::vector<Synapse*> synapse_objects;

/**
 * Maintains the total count of object of Synapse types.
 */
std::vector<unsigned> synapse_objects_count;

/**
 * Generates Neuron objects for any Neuron type and
 * stores it for simulation.
 *
 * @param count An unsigned int count of number
 *              of Neuron type objects to be generated,
 *              defaulted to 1.
 */
template<class T>
void generate_neuron(unsigned count = 1) {
  neuron_objects.push_back(new T());
  neuron_objects_count.push_back(count);
}

/**
 * Generates Synapse objects for any Synapse type and
 * stores it for simulation.
 *
 * @param count An unsigned int count of number
 *              of Synapse type objects to be generated,
 *              defaulted to 1.
 */
template<class T>
void generate_synapse(unsigned count = 1) {
  synapse_objects.push_back(new T());
  synapse_objects_count.push_back(count);
}

/**
 * Iterates over all generated Neuron and Synapse objects.
 *
 * For each type of Neuron and count of each type is considered and
 * iterated over for each simulation integration step or as and when
 * called by Boost.odeint.
 *
 * @param state State type ref for initial variables in simulation.
 * @param dxdt State type ref for ODEs for respective variables in state.
 * @param t Current time in simulation.
 */
void driver::operator()(state_type &state, state_type &dxdt, const double t) {
  unsigned local_index = 0;
  for(std::vector<Neuron*>::size_type type = 0; type < neuron_objects.size(); ++type) {
    for(unsigned iter = 0; iter < neuron_objects_count[type]; ++iter, ++local_index) {
      neuron_objects[type] -> ode_set(state, dxdt, t, local_index);
    }
  }
  local_index = 0;
  for(std::vector<Synapse*>::size_type type = 0; type < synapse_objects.size(); ++type) {
    for(unsigned iter = 0; iter < synapse_objects_count[type]; ++iter, ++local_index) {
      synapse_objects[type] -> ode_set(state, dxdt, t, local_index);
    }
  }
}

} // namespace engine
} // namespace insilico
