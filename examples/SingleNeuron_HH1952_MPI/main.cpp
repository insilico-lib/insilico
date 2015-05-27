/*
  examples/SingleNeuron_HH1952/main.cpp - insilico's example using neuron and
                                          synapse for illustrations

  Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab,
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

#include <boost/numeric/odeint.hpp>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace insilico;
using namespace std;

class I_Na {
 private:
  double gna = 120, ena = 115;
 public:
  void current(state_type &variables, state_type &dxdt,
                      const double t, const unsigned index) {
    unsigned v_index = engine::neuron_index(variables, index, "v");
    unsigned m_index = engine::neuron_index(variables, index, "m");
    unsigned h_index = engine::neuron_index(variables, index, "h");
    
    double v = variables[v_index];
    double m = variables[m_index];
    double h = variables[h_index];

    double alpha_m = (2.5 - 0.1 * v) / (exp(2.5-0.1 * v) - 1.0);
    double beta_m  = 4.0 * exp(-v / 18.0);
    double alpha_h = 0.07 * exp(-v / 20.0);
    double beta_h  = 1.0 / (exp(3 - 0.1 * v) + 1);

    dxdt[m_index] = (alpha_m * (1-m) - beta_m * m);
    dxdt[h_index] = (alpha_h * (1-h) - beta_h * h);

    engine::neuron_value(index, "I_Na", (gna * pow(m, 3) * h * (v - ena)));
  }
};

class I_K {
 private:
  double gk = 36, ek = -12;
 public:
  void current(state_type &variables, state_type &dxdt,
                      const double t, const unsigned index) {
    unsigned v_index = engine::neuron_index(variables, index, "v");
    unsigned n_index = engine::neuron_index(variables, index, "n");

    double v = variables[v_index];
    double n = variables[n_index];

    double alpha_n = (0.1 - 0.01 * v) / (exp(1 - 0.1 * v) - 1.0);
    double beta_n  = 0.125 * exp(-v / 80.0);
    dxdt[n_index] = (alpha_n*(1 - n)-beta_n * n);

    engine::neuron_value(index, "I_K", (gk * pow(n,4) * (v - ek)));
  }
};

class I_Leak {
 private:
  double gl = 0.3, el = 10.6;
 public:
  void current(state_type &variables, state_type &dxdt,
                      const double t, const unsigned index) {
    int v_index = engine::neuron_index(variables, index, "v");
    double v = variables[v_index];

    engine::neuron_value(index, "I_Leak", (gl * (v - el)));
  }
};

class HH_Neuron : public Neuron {
 private:
  I_Na i_na_component;
  I_K i_k_component;
  I_Leak i_leak_component;
 public:
  void ode_set(state_type &variables, state_type &dxdt,
               const double t, const unsigned index) {
    unsigned v_index = engine::neuron_index(variables, index, "v");

    i_na_component.current(variables, dxdt, t, index);
    i_k_component.current(variables, dxdt, t, index);
    i_leak_component.current(variables, dxdt, t, index);

    double I_Na = engine::neuron_value(index, "I_Na");
    double I_K = engine::neuron_value(index, "I_K");
    double I_Leak = engine::neuron_value(index, "I_Leak");
    double I_Ext = engine::neuron_value(index, "I_Ext");

    dxdt[v_index] = - I_Na - I_K - I_Leak + I_Ext;
  }
};

int main(int argc, char **argv) {
  configuration::mpi::initialize(argc, argv);
  configuration::mpi::observe("v");

  engine::generate_neuron<HH_Neuron>(4);

  state_type variables = engine::get_variables();
  integrate_const(boost::numeric::odeint::runge_kutta4<state_type>(),
                  engine::driver(), variables,
                  0.0, 100.0, 0.05, configuration::mpi::observer());

  configuration::mpi::finalize();
}
