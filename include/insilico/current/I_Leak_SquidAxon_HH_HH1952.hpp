/*
 current/I_Leak_SquidAxon_HH_HH1952.hpp - Leak current flowing across neuronal membrane.
                                          (Hodgkin-Huxley, 1952)

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

 Leak current flow across neuronal membrane. (Hodgkin-Huxley, 1952)
*/

#ifndef INCLUDED_I_LEAK_SQUIDAXON_HH_HH1952_HPP
#define INCLUDED_I_LEAK_SQUIDAXON_HH_HH1952_HPP

#include "insilico/core/engine.hpp"

namespace insilico {

class I_Leak_SquidAxon_HH_HH1952 {
 public:
  static void current(state_type &variables, state_type &dxdt, const double t, int index) {
    double gl = 0.3, el = 10.6;

    int v_index = engine::neuron_index(index, "v");
    double v = variables[v_index];

    // Current
    engine::neuron_value(index, "I_Leak_SquidAxon_HH_HH1952", (gl * (v - el)));

  } // function current
}; // class I_Leak_SquidAxon_HH_HH1952

} // insilico

#endif
