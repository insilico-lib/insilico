/*
 current/I_Na_SquidAxon_HH_HH1952.hpp - Current flowing across neuronal membrane due to
                                        Sodium (Na) conductance. (Hodgkin-Huxley, 1952)

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

/*
 Brief:

 Current that flows through Sodium (Na) channel due to the potential difference
 caused by Sodium (Na) conductance across neuronal membrane. (Hodgkin-Huxley, 1952)
*/

#ifndef INCLUDED_I_NA_SQUIDAXON_HH_HH1952_HPP
#define INCLUDED_I_NA_SQUIDAXON_HH_HH1952_HPP

#include "insilico/core/engine.hpp"

namespace insilico {

class I_Na_SquidAxon_HH_HH1952 {
 public:
  static void current(state_type &variables, state_type &dxdt, const double t, int index) {
    double gna = 120, ena = 115;

    int v_index = engine::neuron_index(index, "v");
    int m_index = engine::neuron_index(index, "m");
    int h_index = engine::neuron_index(index, "h");
    
    double v = variables[v_index];
    double m = variables[m_index];
    double h = variables[h_index];
    
    double alpha_m = (2.5-0.1 * v)/(exp(2.5-0.1 * v)-1.0);
    double beta_m  = 4.0*exp(-v /18.0);
    double alpha_h = 0.07*exp(-v / 20.0);
    double beta_h  = 1.0/(exp(3-0.1 * v)+1);

    // ODE set
    dxdt[m_index]=(alpha_m*(1-m)-beta_m*m);
    dxdt[h_index]=(alpha_h*(1-h)-beta_h*h);

    // Current
    engine::neuron_value(index, "I_Na_SquidAxon_HH_HH1952", (gna * pow(m, 3) * h * (v - ena)));

  } // function current
}; // class I_Na_SquidAxon_HH_HH1952

} // insilico

#endif
