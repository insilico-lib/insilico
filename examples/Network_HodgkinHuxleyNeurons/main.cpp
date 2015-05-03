/*
  main.cpp - insilico's example using neuron and synapse for illustrations

  Copyright (C) 2014 Collins Assisi, Collins Assisi Lab, IISER, Pune
  Copyright (C) 2014 Arun Neru, Collins Assisi Lab,
                     IISER, Pune <areinsdel@gmail.com>
  Copyright (C) 2014-2015 Pranav Kulkarni, Collins Assisi Lab,
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

#include "insilico/core.hpp"

#include "insilico/neuron/helper/spike_list.hpp" //storing spike-list
#include "insilico/neuron/N_SquidAxon_HH1952.hpp"
#include "insilico/synapse/S_DefaultSynapse.hpp"

#include <boost/numeric/odeint.hpp>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace insilico;
using namespace std;

int main(int argc, char **argv) {
  configuration::initialize(argc, argv);
  configuration::observe("v");

  engine::generate_neuron<N_SquidAxon_HH1952>(2);
  engine::generate_synapse<S_DefaultSynapse>(2);
  engine::spike_list.resize(2);

  state_type variables = engine::get_variables();
  integrate_const(boost::numeric::odeint::runge_kutta4<state_type>(),
                  engine::driver(), variables,
                  0.0, 100.0, 0.05, configuration::observer());

  configuration::finalize();
}
