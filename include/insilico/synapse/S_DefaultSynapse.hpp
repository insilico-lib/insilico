/*
 synapse/S_DefaultSynapse.hpp - Default non-specialized synapse with last spike logic

 Copyright (C) 2014 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

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

#ifndef INCLUDED_S_DEFAULTSYNAPSE_HPP
#define INCLUDED_S_DEFAULTSYNAPSE_HPP

#include "insilico/core/engine.hpp"

namespace insilico {

class S_DefaultSynapse : public Synapse {
 public:
  void ode_set(state_type &variables, state_type &dxdt, const double t, unsigned index) {
    int g1_index = engine::synapse_index(index, "g1");
    int g2_index = engine::synapse_index(index, "g2");

    double g1 = variables[g1_index];
    double g2 = variables[g2_index];

    // synapse logic for delay for recently spiked neuron
    double xt = 0.0;
    double delay = engine::synapse_value(index, "delay");

    int neuron_index = engine::synapse_value(index, "post");
    double last_spiked = engine::neuron_value(neuron_index, "last_spike");

    if((t - last_spiked) > delay){
      xt = 1.0;
    }

    // constants from file
    double tau1 = engine::synapse_value(index, "tau1");
    double tau2 = engine::synapse_value(index, "tau2");
    double gsyn = engine::synapse_value(index, "gsyn");

    // ODE set
    dxdt[g1_index] = g2;
    dxdt[g2_index] = (gsyn * xt - g1 - (tau1+tau2) * g2) / (tau1*tau2);

  } // function ode_set
}; // class S_DefaultSynapse

} // insilico

#endif
