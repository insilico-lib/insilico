/*
  core/engine/serial.hpp - insilico engine serial API header and source

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

#ifndef INCLUDED_INSILICO_CORE_ENGINE_GENERATE_HPP
#define INCLUDED_INSILICO_CORE_ENGINE_GENERATE_HPP

#include "core/type.hpp"

#include <boost/numeric/odeint.hpp>

#include <iostream>
#include <vector>

namespace insilico { namespace engine {

std::vector< Neuron* > neuron_objects;
std::vector< int > neuron_objects_count;

std::vector< Synapse* > synapse_objects;
std::vector< int > synapse_objects_count;

template<class T>
auto generate_neurons(unsinged count = 1) -> void {
  neuron_objects.push_back(new T());
  neuron_objects_count.push_back(count);
}

template<class T>
auto generate_synapse(unsinged count = 1) -> void {
  synapse_objects.push_back(new T());
  synapse_objects_count.push_back(count);
}

void engine::driver::operator()(state_type &_state, state_type &_dxdt, const double time) {
  for(std::vector<Neuron*>::size_type type = 0; type < neuron_objects.size(); ++type) {
    for(int iter = 0; iter < neuron_objects_count[type]; ++iter) {
      neuron_objects[type] -> ode_set(_state, _dxdt, time, iter);
    }
  }
  for(std::vector<Synapse*>::size_type type = 0; type < synapse_objects.size(); ++type) {
    for(int j = 0; j < synapse_objects_count[type]; ++j) {
      synapse_objects[type] -> ode_set(_state, _dxdt, time, iter);
    }
  }
}

} } // namespace insilico::engine

#endif
