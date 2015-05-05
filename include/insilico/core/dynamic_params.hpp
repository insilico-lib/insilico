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
#include "insilico/core/helper/floating_comp.hpp"

#include <cmath>

namespace insilico { namespace dynamic_params {

auto value(const std::string& _param, const double _time)
    -> double {
  double param_val = 0;
  if(!time_seq.empty()) {
    auto param_it = dynamic_params_seq.find(_param);
    if(isequal(time_seq.front(), _time)) {
      return param_it->second[0];
    }
    if(isequal(time_seq.back(), _time)) {
      return param_it->second[time_seq.size() - 1];
    }
    if(_time >= time_seq.front() && _time <= time_seq.back()) {
      auto loc = std::upper_bound(time_seq.begin(), time_seq.end(), _time);
      unsigned idx = loc - time_seq.begin();
      if(isequal(time_seq[idx - 1], _time)) {
        if(param_it != dynamic_params_seq.end()) {
          param_val = param_it->second[idx - 1];
        }
      }
    }
  }
  return param_val;
}

} } // namespace insilico::dynamic_params
  
#endif
