/*
  core/engine/serial/common.hpp - Common data handlers for engine header and source

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_SERIAL_COMMON_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_SERIAL_COMMON_HPP

#include "insilico/core/type.hpp"
#include "insilico/core/engine/data.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico { namespace engine {

inline auto get_variables() -> state_type {
  return var_vals;
}

inline auto neuron_count() -> int {
  return neuron_start_list_ids.size();
}

inline auto synapse_count() -> int {
  return synapse_start_list_ids.size();
}

} } // namespace insilico::engine

#endif
