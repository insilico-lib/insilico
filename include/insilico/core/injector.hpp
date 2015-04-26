/*
 core/injector.hpp - insilico's external current injector header and source

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
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDED_INSILICO_CORE_INJECTOR_HPP
#define INCLUDED_INSILICO_CORE_INJECTOR_HPP

#include "insilico/core/injector/parser.hpp"

#include <cmath>

namespace insilico { namespace injector {

std::vector< double > external_current(const int _neuron_id, const double _time) {
  std::vector< double > current_values;
  long index = -1;
  for(unsigned time_id = 0; time_id < injector::time_seq.size(); ++time_id) {
    if(std::abs(injector::time_seq[time_id] - _time) < 0.001) {
      index = time_id;
      break;
    }
  }
  if(index >= 0) {
    for(unsigned current_id = 0; current_id < injector::neurons_seq.size(); ++current_id) {
      if(_neuron_id == injector::neurons_seq[current_id]) {
        current_values.push_back(injector::external_current_seq[_neuron_id][index]);
      }
    }
  }
  return current_values;
}

} } // namespace insilico::injector
  
#endif
