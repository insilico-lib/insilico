/*
  core/engine/driver.hpp - Solver trigger for integration and non-integration

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
 * File: core/engine/driver.hpp
 *
 * Declaration specific to insilico's engine solver triggering by
 * Boost.odeint
 */

#pragma once

#include "insilico/core/type.hpp"

#include <iostream>
#include <vector>

namespace insilico {
namespace engine {

/**
 * Handles Boost.odeint integration specific internal calls
 */
class driver {
 public:

  /**
   * Operator overload for Boost.odeint integrator call
   */
  void operator()(state_type &variables, state_type &dxdt, const double time);
};

} // namespace engine
} // namespace insilico
