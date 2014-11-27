/*
    main.cpp - nsim source supporting models and main()

    Copyright (C) 2014 Collins Assisi
    Copyright (C) 2014 Pranav Kulkarni

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

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/support/pair.hpp>
#include <boost/numeric/odeint.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
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
    long v_index = nnet::get_index(index, "v", NEURON);
    long m_index = nnet::get_index(index, "m", NEURON);
    long h_index = nnet::get_index(index, "h", NEURON);
    
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
    long v_index = nnet::get_index(index, "v", NEURON);
    long n_index = nnet::get_index(index, "n", NEURON);

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
    long v_index = nnet::get_index(index, "v", NEURON);
    long n_index = nnet::get_index(index, "n", NEURON);
    long m_index = nnet::get_index(index, "m", NEURON);
    long h_index = nnet::get_index(index, "h", NEURON);
        
    double v = variables[v_index];
    double n = variables[n_index];
    double m = variables[m_index];
    double h = variables[h_index];
    
    double gna = 120, ena = 115, gk = 36, ek = -12, gl = 0.3, el = 10.6;
    double iext = nnet::get(index, "iext", NEURON);
  
    vector<long> g1_indices = nnet::get_indices(index, "g1", SYNAPSE);
    vector<long> esyn_indices = nnet::get_indices(index, "esyn", SYNAPSE);    
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
    long g1_index = nnet::get_index(index, "g1", SYNAPSE);
    long g2_index = nnet::get_index(index, "g2", SYNAPSE);

    double g1 = variables[g1_index];
    double g2 = variables[g2_index];

    double tau1 = nnet::get(index, "tau1", SYNAPSE);
    double tau2 = nnet::get(index, "tau2", SYNAPSE);
    double gsyn = nnet::get(index, "gsyn", SYNAPSE);

    double xt = 1;
    dxdt[g1_index] = g2;
    dxdt[g2_index] = -((tau1+tau2)/(tau1*tau2))*g2-g1+gsyn*xt; 
  }
};

void nnet::operator()(const state_type &variables, state_type &dxdt,
                       const double time) {
  // spawn the mpi procs
  long network_size = nnet::neuron_count();
  long synapse_count = nnet::synapse_count();

  // divide the rank+1 jobs here
  for(long neuron_index = 0; neuron_index < network_size; ++neuron_index) {
    hodgkin_huxley_neuron::ode_set(variables, dxdt, time, neuron_index);
  }
  // seperate synapse division
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
  assert(output_file.is_open()); // safety check
  
  using namespace boost::numeric::odeint;
  integrate_const(runge_kutta4<state_type>(), network, variables,
                  0.0, 100.0, 0.05, configuration(output_file));

  output_file.close();
  return 0;
}
