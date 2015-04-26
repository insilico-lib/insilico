/*
  main.cpp - Integrate and Fire neuron example's main()

  Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab,
                     IISER, Pune <pranavcode@gmail.com>
  Copyright (C) 2015 Himanshu Rajmane, Suhita Nadkarni Lab,
                     IISER, Pune <himanshu14121992@gmail.com>

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

#include "insilico/core.hpp"

#include "insilico/neuron/helper/spike_list.hpp"
#include "insilico/neuron/N_LIF_S1967.hpp"
#include "insilico/synapse/S_LIF_Synapse.hpp"

#include <boost/numeric/odeint.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace boost;
using namespace insilico;
using namespace std;

int main(int argc, char **argv) {
  configuration::initialize(argc, argv);
  configuration::observe("v");

  // 2 N_LIF_S1967 neurons
  engine::generate_neuron<N_LIF_S1967>(2);
  // Single S_LIF_Synapse synapse between our two LIF neurons
  engine::generate_synapse<S_LIF_Synapse>();

  // Spike list maintains neuron specific spike information
  // In our case we have two neurons
  engine::spike_list.resize(2);

  state_type variables = engine::get_variables();
  integrate_const(boost::numeric::odeint::runge_kutta4<state_type>(),
                  engine::driver(), variables,
                  0.0, 50.0, 0.01, configuration::observer());

  configuration::finalize();
}
