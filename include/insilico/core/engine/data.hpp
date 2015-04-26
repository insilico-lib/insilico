/*
  core/engine/data.hpp - insilico engine data header

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_DATA_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_DATA_HPP

#include "insilico/core/type.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico { namespace engine {

std::unordered_map< std::string, unsigned > index_map;

std::unordered_map< std::string, double > value_map;

std::vector< unsigned > neuron_start_list_ids;

std::vector< unsigned > neuron_end_list_ids;

std::vector< unsigned > synapse_start_list_ids;

std::vector< unsigned > synapse_end_list_ids;

std::vector< std::vector< unsigned > > pre_synaptic_lists;

std::vector< unsigned > pre_neuron;

std::vector< unsigned > post_neuron;

std::vector< std::string > var_list_ids;

state_type var_vals;

} } //namespace insilico::engine

#endif
