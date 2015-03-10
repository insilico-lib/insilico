/*
  core/engine.hpp - insilico's Simulation Engine API header and source

  Copyright (C) 2014-2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_HPP

#include "core/type.hpp"

#ifdef INSILICO_MPI_ENABLE
#include "parallel/synchronization.hpp"
#else
#include "core/engine/serial.hpp"
#endif

#include <iostream>
#include <vector>

namespace insilico { namespace engine {

class driver {
 public:
  void operator()(state_type &variables, state_type &dxdt, const double time);
};

} } // namespace insilico::engine

#endif
