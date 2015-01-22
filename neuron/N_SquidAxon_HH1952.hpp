/*
 neuron/N_SquidAxon_HH1952.hpp - Hodgkin-Huxley Squid Axon experiment (Hodgkin-Huxley, 1952)

 Copyright (C) 2014 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>
 Copyright (C) 2014 Nishant Singh, Suhita Nadkarni Lab, IISER, Pune <nishant.singh@students.iiserpune.ac.in>

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

#ifndef INCLUDED_N_SQUIDAXON_HH1952_HPP
#define INCLUDED_N_SQUIDAXON_HH1952_HPP

#include "core/engine.hpp"

#include "current/I_K_SquidAxon_HH_HH1952.hpp"
#include "current/I_Leak_SquidAxon_HH_HH1952.hpp"
#include "current/I_Na_SquidAxon_HH_HH1952.hpp"

namespace insilico {

class N_SquidAxon_HH1952 {
 public:
  static void ode_set(state_type& variables, state_type& dxdt, const double t,
               int index) {
    int v_index = engine::neuron_index(index, "v");
    double v = variables[v_index];

    vector<int> g1_indices = engine::get_pre_neuron_indices(index, "g1");
    vector<int> esyn_values = engine::get_pre_neuron_values(index, "esyn");

    // note the spike
    double last_spiked = engine::neuron_value(index, "last_spike");
    double spike_duration = engine::neuron_value(index, "spike_duration");
    double thresh = engine::neuron_value(index, "thresh");

    // associated delay for next spikes
    if((v > thresh) && (t - last_spiked) > spike_duration){
      engine::neuron_value(index, "last_spike", t);
    }

    // incoming synaptic currents
    double I_Syn = 0;
    for(vector<int>::size_type iterator = 0; iterator < g1_indices.size(); ++iterator) {
      I_Syn = I_Syn + variables[g1_indices[iterator]] * (v - esyn_values[iterator]);
    }

    // ODE set
    I_Na_SquidAxon_HH_HH1952::current(variables, dxdt, t, index);
    I_K_SquidAxon_HH_HH1952::current(variables, dxdt, t, index);
    I_Leak_SquidAxon_HH_HH1952::current(variables, dxdt, t, index);

    double I_Na = engine::current_value(index, "I_Na_SquidAxon_HH_HH1952");
    double I_K = engine::current_value(index, "I_K_SquidAxon_HH_HH1952");
    double I_Leak = engine::current_value(index, "I_Leak_SquidAxon_HH_HH1952");
    double I_Ext = engine::neuron_value(index, "iext");

    dxdt[v_index] = I_Na + I_K + I_Leak + I_Ext + I_Syn;
  } // function ode_set
}; // class N_SquidAxon_HH1952

} // insilico

#endif
