/*
  core/engine/data.hpp - insilico engine data header

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
 * File: core/engine/data.hpp
 *
 * The collection of data stores used by insilico's engine component.
 */

#pragma once

#include "insilico/core/type.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico {
namespace engine {

/**
* Value that specifies the length of the simulation
*/
double simulation_time;

/**
*Value that specifies the magnitudde of the time step for integration
*/
double time_step;
 
/**
 * List of values for each variable from input files
 */
state_type var_vals;

/**
 * List of names for each variable from input files
 */
std::vector<std::string> var_list_ids;

/**
 * Stores the map of variable index
 * key - variable name
 * value - variable index
 */
std::unordered_map<std::string, unsigned> index_map;

/**
 * Stores the map of initial values for variables and parameters
 * key - variable/parameter name
 * value - initial variable/parameter value supplied via input file
 */
std::unordered_map<std::string, double> value_map;

/**
 * List of indices of corresponding first variable
 * for each Neuron
 */
std::vector<unsigned> neuron_start_list_ids;

/**
 * List of immediate next indices of corresponding
 * last variable for each Neuron
 */
std::vector<unsigned> neuron_end_list_ids;

/**
 * List of indices of corresponding first variable
 * for each Synapse
 */
std::vector<unsigned> synapse_start_list_ids;

/**
 * List of immediate next indices of corresponding
 * last variable for each Synapse
 */
std::vector<unsigned> synapse_end_list_ids;

/**
 * Neuron IDs are pre-populated against
 * each Neuron variable's index
 */
std::vector<unsigned> prepopulated_neuron_ids;

/**
 * Synapse IDs are pre-populated againt
 * each Synapse variable's index
 */
std::vector<unsigned> prepopulated_synapse_ids;

/**
 * Lists the pre-synaptic Neuron IDs for each Neuron ID
 */
std::vector<std::vector<unsigned>> pre_synaptic_lists;

/**
 * Pre-synaptic Neuron ID for Neuron ID referred
 * with index for this list
 * E.g. pre_neuron[7] refers to Neuron ID 7 and
 *      tries to fetch pre-synaptic Neuron ID for
 *      that Neuron
 */
std::vector<unsigned> pre_neuron;

/**
 * Post-synaptic Neuron ID for Neuron ID referred
 * with index for this list
 * E.g. post_neuron[7] refers to Neuron ID 7 and
 *      tries to fetch post-synaptic Neuron ID for
 *      that Neuron
 */
std::vector< unsigned > post_neuron;

} // namespace engine
} // namespace insilico
