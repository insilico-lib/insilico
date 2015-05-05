/*
 core/injector.hpp - insilico's external current injector header and source

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

#ifndef INCLUDED_INSILICO_CORE_INJECTOR_HPP
#define INCLUDED_INSILICO_CORE_INJECTOR_HPP

#include "insilico/core/injector/parser.hpp"
#include "insilico/core/helper/floating_comp.hpp"

#include <cmath>

namespace insilico { namespace injector {

auto external_current(const unsigned _id, const double _time)
    -> std::vector<double> {
  std::vector<double> currents;
  unsigned index = 0;
  bool found = false;
  if(!time_seq.empty()) {
    if(isequal(time_seq.front(), _time)) {
      index = 0; found = true;
    }
    else if(isequal(time_seq.back(), _time)) {
      index = time_seq.size() - 1; found = true;
    }
    else if(_time >= time_seq.front() && _time <= time_seq.back()) {
      auto loc = std::upper_bound(time_seq.begin(), time_seq.end(), _time);
      unsigned idx = loc - time_seq.begin();
      if(isequal(time_seq[idx - 1], _time)) {
        index = idx - 1; found = true;
      }
    }
    if(found) {
      auto bloc = std::lower_bound(neurons_seq.begin(), neurons_seq.end(), _id);
      auto eloc = std::upper_bound(neurons_seq.begin(), neurons_seq.end(), _id);
      for(auto id = (bloc - neurons_seq.begin());
          id < (eloc - neurons_seq.begin()); ++id) {
        currents.push_back(external_current_seq[id][index]);
      }
    }
  }
  return currents;
}

} } // namespace insilico::injector
  
#endif
