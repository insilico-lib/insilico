/*
  core/engine/serial/common.hpp - Common API for engine's data store

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
 * @file core/engine/serial/common.hpp
 *
 * Common functions for accessing data in engine's data store
 */

#pragma once

#include "insilico/core/type.hpp"
#include "insilico/core/engine/data.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico {
namespace engine {

/**
 * Returns the list of values for variables from input files.
 */
inline state_type get_variables() {
  return var_vals;
}

/**
 * Returns the total number of Neurons in the simulation.
 */
inline int neuron_count() {
  return (prepopulated_neuron_ids.empty())?0:(prepopulated_neuron_ids.back() + 1);
}

/**
 * Returns the total number of Synapses in the simulation.
 */
inline int synapse_count() {
  return (prepopulated_synapse_ids.empty())?0:(prepopulated_synapse_ids.back() + 1);
}

} // namespace engine
} // namespace insilico
