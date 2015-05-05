/*
 core/helper/floating_comp.hpp - insilico's floating point comparison
                                 helper source

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
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDED_INSILICO_CORE_HELPER_FLOATING_COMP_HPP
#define INCLUDED_INSILICO_CORE_HELPER_FLOATING_COMP_HPP

#include <cmath>

namespace insilico {

/* Floating point approximation of equal values */
const double __acceptable_error = 1e-5;
auto isequal(const double first, const double second) -> bool {
  if(std::abs(first - second) <= (__acceptable_error * std::abs(first))) {
    return true;
  }
  return false;
}

} // namespace insilico

#endif

