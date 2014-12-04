/*
 main.cpp - nsim source supporting models and main()

 Copyright (C) 2014 Collins Assisi, Collins Assisi Lab, IISER, Pune
 Copyright (C) 2014 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>
 Copyright (C) 2014 Arun Neru, Collins Assisi Lab, IISER, Pune <areinsdel@gmail.com>

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

#include <boost/numeric/odeint.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <nsim/network/nnet.hpp>

using namespace boost;
using namespace std;

typedef vector<long double> state_type;

struct configuration {
  ofstream &stream;
  configuration(ofstream &file): stream(file) {}
  void operator()(const state_type &variables, const double t) {
    vector<long> indices = nnet::get_indices("v");
    assert(stream.is_open());
    stream<<t;
    for(vector<long>::size_type iter = 0; iter < indices.size(); ++iter) {
      stream<<','<<variables[indices[iter]];
    }
    stream<<endl;
  }
};

class na_conductance {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t, long index) { 
    long v_index = nnet::neuron_index(index, "v");
    long m_index = nnet::neuron_index(index, "m");
    long h_index = nnet::neuron_index(index, "h");
    
    double v = variables[v_index];
    double m = variables[m_index];
    double h = variables[h_index];
    
    double alpha_m = (2.5-0.1 * v)/(exp(2.5-0.1 * v)-1.0);
    double beta_m  = 4.0*exp(-v /18.0);
    double alpha_h = 0.07*exp(-v / 20.0);
    double beta_h  = 1.0/(exp(3-0.1 * v)+1);

    dxdt[m_index]=(alpha_m*(1-m)-beta_m*m);
    dxdt[h_index]=(alpha_h*(1-h)-beta_h*h);
  }
};

class k_conductance {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t, long index) {
    long v_index = nnet::neuron_index(index, "v");
    long n_index = nnet::neuron_index(index, "n");

    double v = variables[v_index];
    double n = variables[n_index];

    double alpha_n = (0.1-0.01 * v)/(exp(1-0.1 * v)-1.0);
    double beta_n = 0.125*exp(-v / 80.0);

    dxdt[n_index]=(alpha_n*(1 - n)-beta_n * n);
  }
};

class leak_conductance {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t, long index) {}
};

class hodgkin_huxley_neuron {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t,
               long index) {
    long v_index = nnet::neuron_index(index, "v");
    long n_index = nnet::neuron_index(index, "n");
    long m_index = nnet::neuron_index(index, "m");
    long h_index = nnet::neuron_index(index, "h");

    double v = variables[v_index];
    double n = variables[n_index];
    double m = variables[m_index];
    double h = variables[h_index];
    
    double gna = 120, ena = 115, gk = 36, ek = -12, gl = 0.3, el = 10.6;
    double iext = nnet::neuron_value(index, "iext");
  
    vector<long> g1_indices = nnet::get_pre_neuron_indices(index, "g1", SYNAPSE);
    vector<long> esyn_indices = nnet::get_pre_neuron_indices(index, "esyn", SYNAPSE);
    double isyn = 0;

    for(vector<long>::size_type iterator = 0; iterator < g1_indices.size(); ++iterator) {
      isyn = isyn + variables[g1_indices.at(iterator)] * (v - variables[esyn_indices.at(iterator)]); 
    }

    // ODE
    dxdt[v_index] = -gna*pow(m,3)*h*(v-ena)-gk*pow(n,4)*(v-ek)-gl*(v-el)+iext+isyn;

    k_conductance::ode_set(variables, dxdt, t, index);
    na_conductance::ode_set(variables, dxdt, t, index);
    leak_conductance::ode_set(variables, dxdt, t, index);
  }
};

class synapse_x {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t,
               long index) {
    long g1_index = nnet::synapse_index(index, "g1");
    long g2_index = nnet::synapse_index(index, "g2");
    long pre_index = nnet::synapse_index(index, "pre");
    long neuron_index = variables[pre_index];

    long last_spiked_index = nnet::synapse_index(index, "last_spike");
    long v_index = nnet::neuron_index(neuron_index, "v");

    double g1 = variables[g1_index];
    double g2 = variables[g2_index];
    double last_spiked = variables[last_spiked_index];
    double V = variables[v_index];

    double def_delay = .004, thresh = 20.0, xt = 0.0; // constants to function

    // synapse logic for decay
    if((V > thresh) && (t-last_spiked)>def_delay){
      xt = 1.0;
      dxdt[last_spiked_index] = t*(1.0/0.05);
    }

    // constants from file
    double tau1 = nnet::synapse_value(index, "tau1");
    double tau2 = nnet::synapse_value(index, "tau2");
    double gsyn = nnet::synapse_value(index, "gsyn");

    dxdt[g1_index] = g2;
    dxdt[g2_index] = -((tau1+tau2)/(tau1*tau2))*g2-g1+gsyn*xt;
  }
};

void nnet::operator()(const state_type &variables, state_type &dxdt,
                       const double time) {
  long network_size = nnet::neuron_count();
  long synapse_count = nnet::synapse_count();

  for(long neuron_index = 0; neuron_index < network_size; ++neuron_index) {
    hodgkin_huxley_neuron::ode_set(variables, dxdt, time, neuron_index);
  }
  for(long synapse_index = 0; synapse_index < synapse_count; ++synapse_index) {
    synapse_x::ode_set(variables, dxdt, time, synapse_index);
  }
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    cout<<"Usage: "<<argv[0]<<" <outputfile>.dat"<<endl;
    return -1;
  }
  
  nnet network;
  nnet::read("nsets.conf","ssets.conf");
  state_type variables = nnet::get_variables();
  string output_filename = argv[1];
  ofstream output_file(output_filename);
  assert(output_file.is_open());
  
  using namespace boost::numeric::odeint;
  integrate_const(runge_kutta4<state_type>(), network, variables,
                  0.0, 100.0, 0.05, configuration(output_file));

  output_file.close();
  return 0;
}
