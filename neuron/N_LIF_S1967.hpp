/*
 neuron/N_LIF_S1967.hpp - Leaky Integrate and Fire (Stein, 1967)

 Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>
 Copyright (C) 2015 Himanshu Rajmane, Suhita Nadkarni Lab, IISER, Pune <himanshu14121992@gmail.com>

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

#ifndef INCLUDED_N_LIF_S1967_HPP
#define INCLUDED_N_LIF_S1967_HPP

#include "core/engine.hpp"

namespace insilico {

class N_LIF_S1967 {
 public:
  static void ode_set(state_type& variables, state_type& dxdt, const double t,
               int index) {
    int v_index = engine::neuron_index(index, "v");

    double v = variables[v_index];
    double g = engine::neuron_value(index, "g");
    double v_th = engine::neuron_value(index, "v_th");
    double iext = engine::neuron_value(index, "iext");
    double tau_m = engine::neuron_value(index, "tau_m");
    double t_rest = engine::neuron_value(index, "t_rest");
    double tau_ref = engine::neuron_value(index, "tau_ref");    
    
    // ODE set
    if(t > t_rest) {
      dxdt[v_index] = (-v + iext / g) / tau_m;
      if(v >= v_th) {
        engine::neuron_value(index, "t_rest" , t + tau_ref);
      }
    }
    else {
      variables[v_index] = 0;
    }    
  } // function ode_set
}; // class N_LIF_S1967

} // namespace insilico

#endif
