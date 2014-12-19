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

#include "network/nnet.hpp"

#include <boost/numeric/odeint.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace boost;
using namespace std;

class na_conductance {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t, int index) { 
    int v_index = nnet::neuron_index(index, "v");
    int m_index = nnet::neuron_index(index, "m");
    int h_index = nnet::neuron_index(index, "h");
    
    float v = variables[v_index];
    float m = variables[m_index];
    float h = variables[h_index];
    
    float alpha_m = (2.5-0.1 * v)/(exp(2.5-0.1 * v)-1.0);
    float beta_m  = 4.0*exp(-v /18.0);
    float alpha_h = 0.07*exp(-v / 20.0);
    float beta_h  = 1.0/(exp(3-0.1 * v)+1);

    dxdt[m_index]=(alpha_m*(1-m)-beta_m*m);
    dxdt[h_index]=(alpha_h*(1-h)-beta_h*h);
  }
};

class k_conductance {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t, int index) {
    int v_index = nnet::neuron_index(index, "v");
    int n_index = nnet::neuron_index(index, "n");

    float v = variables[v_index];
    float n = variables[n_index];

    float alpha_n = (0.1-0.01 * v)/(exp(1-0.1 * v)-1.0);
    float beta_n = 0.125*exp(-v / 80.0);

    dxdt[n_index]=(alpha_n*(1 - n)-beta_n * n);
  }
};

class leak_conductance {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t, int index) {}
};

class hodgkin_huxley_neuron {
 public:
  static void ode_set(const state_type &variables, state_type &dxdt, const double t,
               int index) {
    int v_index = nnet::neuron_index(index, "v");
    int n_index = nnet::neuron_index(index, "n");
    int m_index = nnet::neuron_index(index, "m");
    int h_index = nnet::neuron_index(index, "h");

    float v = variables[v_index];
    float n = variables[n_index];
    float m = variables[m_index];
    float h = variables[h_index];
    
    float gna = 120, ena = 115, gk = 36, ek = -12, gl = 0.3, el = 10.6;
    float iext = nnet::neuron_value(index, "iext");
  
    vector<int> g1_indices = nnet::get_pre_neuron_indices(index, "g1");
    vector<int> esyn_values = nnet::get_pre_neuron_values(index, "esyn");
    float isyn = 0;

    for(vector<int>::size_type iterator = 0; iterator < g1_indices.size(); ++iterator) {
      isyn = isyn + variables[g1_indices[iterator]] * (v - esyn_values[iterator]);
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
               int index) {
    int g1_index = nnet::synapse_index(index, "g1");
    int g2_index = nnet::synapse_index(index, "g2");
    int neuron_index = nnet::synapse_value(index, "pre");

    int last_spiked_index = nnet::synapse_index(index, "last_spike");
    int v_index = nnet::neuron_index(neuron_index, "v");

    float g1 = variables[g1_index];
    float g2 = variables[g2_index];
    float last_spiked = variables[last_spiked_index];
    float V = variables[v_index];

    float def_delay = .004, thresh = 20.0, xt = 0.0; // constants to function

    // synapse logic for decay
    if((V > thresh) && (t-last_spiked)>def_delay){
      xt = 1.0;
      dxdt[last_spiked_index] = t*(1.0/0.05);
    }

    // constants from file
    float tau1 = nnet::synapse_value(index, "tau1");
    float tau2 = nnet::synapse_value(index, "tau2");
    float gsyn = nnet::synapse_value(index, "gsyn");

    dxdt[g1_index] = g2;
    dxdt[g2_index] = -((tau1+tau2)/(tau1*tau2))*g2-g1+gsyn*xt;
  }
};

void nnet::operator()(const state_type &variables, state_type &dxdt,
                       const double time) {
  int network_size = nnet::neuron_count();
  int synapse_count = nnet::synapse_count();

  for(int neuron_index = 0; neuron_index < network_size; ++neuron_index) {
    hodgkin_huxley_neuron::ode_set(variables, dxdt, time, neuron_index);
  }
  for(int synapse_index = 0; synapse_index < synapse_count; ++synapse_index) {
    synapse_x::ode_set(variables, dxdt, time, synapse_index);
  }
}

struct configuration {
  ofstream &stream;
  configuration(ofstream &file): stream(file) {}
  void operator()(const state_type &variables, const double t) {
    vector<int> indices = nnet::get_indices("v");
    assert(stream.is_open());
    stream<<t;
    for(vector<int>::size_type iter = 0; iter < indices.size(); ++iter) {
      stream<<','<<variables[indices[iter]];
    }
    stream<<endl;
  }
};

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
