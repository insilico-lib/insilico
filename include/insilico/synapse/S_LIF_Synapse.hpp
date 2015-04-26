/*
 synapse/S_LIF_Synapse.hpp - Synapse with last spike logic
                             Written to accompany LIF neurons

 Copyright (C) 2014 Pranav Kulkarni, Collins Assisi Lab,
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

#ifndef INCLUDED_S_LIF_SYNAPSE_HPP
#define INCLUDED_S_LIF_SYNAPSE_HPP

#include "insilico/core/engine.hpp"
#include "insilico/neuron/helper/spike_list.hpp"

#include <cmath>

namespace insilico {

class S_LIF_Synapse : public Synapse {
 public:
  void ode_set(state_type &variables, state_type &dxdt,
               const double t, unsigned index) {
    // Synapse logic derived by converting second order
    // differential equation f(g) into two single order
    // differential equations f(g1) and f(g2)

    // Synapse logic for delay for recently spiked neuron
    auto xt = 0.0;
    auto delay = engine::synapse_value(index, "delay");
    auto utilized_spike = engine::synapse_value(index, "us");
    auto neuron_index = engine::synapse_value(index, "pre");
    auto spike_list = engine::spike_list[neuron_index];

    for(double time : spike_list) {
       if(t - time >= delay && utilized_spike < time) {         
          xt = 1.0;
          engine::synapse_value(index,"us", time);
          break;
       }
    }

    // Variables from file
    auto g1_index = engine::synapse_index(index, "g1");
    auto g1 = variables[g1_index];
    auto g2_index = engine::synapse_index(index, "g2");
    auto g2 = variables[g2_index];
    
    // Parameters from file
    auto tau_psp = engine::synapse_value(index, "tau_psp");
    auto gsyn = engine::synapse_value(index, "gsyn");

    // ODE
    dxdt[g1_index] = g2;
    dxdt[g2_index] = -(2*tau_psp*g2+g1-gsyn*xt)/(tau_psp*tau_psp);
  } // function ode_set
}; // class S_LIF_Synapse

} // insilico

#endif
