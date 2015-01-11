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

#include "core/engine.hpp"

namespace insilico {

class S_DefaultSynapse {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t,
               int index) {
    int g1_index = engine::synapse_index(index, "g1");
    int g2_index = engine::synapse_index(index, "g2");
    int neuron_index = engine::synapse_value(index, "pre");

    int last_spiked_index = engine::synapse_index(index, "last_spike");
    int v_index = engine::neuron_index(neuron_index, "v");

    double g1 = variables[g1_index];
    double g2 = variables[g2_index];
    double last_spiked = variables[last_spiked_index];
    double V = variables[v_index];

    double def_delay = 0.3, thresh = 20.0, xt = 0.0; // constants to function

    // synapse logic for decay
    if((V > thresh) && (t-last_spiked)>def_delay){
      xt = 1.0;
      dxdt[last_spiked_index] = t*(1.0/0.05);
    }

    // constants from file
    double tau1 = engine::synapse_value(index, "tau1");
    double tau2 = engine::synapse_value(index, "tau2");
    double gsyn = engine::synapse_value(index, "gsyn");

    // ODE set
    dxdt[g1_index] = g2;
    dxdt[g2_index] = -((tau1+tau2)/(tau1*tau2))*g2-g1+gsyn*xt;

  } // function ode_set
}; // class S_DefaultSynapse

} // insilico

#endif


