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

#include "core/injector/parser.hpp"

namespace insilico { namespace injector {

std::vector<double> external_current(const int neuron_id, const double time) {
  std::vector<double> current_values;
  unsigned index = (std::find(injector::time_seq.begin(), injector::time_seq.end(), time) - injector::time_seq.begin());
  if(index > injector::time_seq.size()) {
    std::cerr << "[insilico::injector] External current for neuron " << neuron_id
              << " at time "<< time << " does not exist.\n";
    exit(1);
  }
  
  for(std::vector<int>::size_type id = 0; id < injector::neurons_seq.size(); ++id) {
    if(neuron_id == injector::neurons_seq[id]) {
      current_values.push_back(injector::external_current_seq[neuron_id][index]);
    }    
  }
  return current_values;
}

} } // namespace insilico::injector
  
#endif
