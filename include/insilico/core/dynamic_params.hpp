/*
 core/dynamic_params.hpp - insilico's dynamic parameters supplier

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

#ifndef INCLUDED_INSILICO_CORE_DYNAMIC_PARAMS_HPP
#define INCLUDED_INSILICO_CORE_DYNAMIC_PARAMS_HPP

#include "insilico/core/dynamic_params/parser.hpp"

#include <cmath>

namespace insilico { namespace dynamic_params {

auto value(const std::string& _param, const double _time)
    -> double {
  const double desired_error = 1e-5;
  double param_val = 0;
  unsigned index = 0, time_id = 0;
  for(time_id = 0; time_id < time_seq.size();
      ++time_id) {
    if(std::abs(time_seq[time_id] - _time) <=
       desired_error * std::abs(time_seq[time_id])) {
      index = time_id;
      break;
    }
  }
  if(time_id < time_seq.size()) {
    auto iterator = dynamic_params_seq.find(_param);
    if(iterator != dynamic_params_seq.end()) {
      param_val = iterator->second[index];
    }
  }
  return param_val;
}

} } // namespace insilico::dynamic_params
  
#endif
