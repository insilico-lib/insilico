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
#include <array>

#define NEURON 1
#define SYNAPSE 2

using namespace boost;
using namespace std;

typedef std::vector<long double> state_type;

typedef class neuronal_network {
 public:
  static vector<long> neuron_start_list_ids, neuron_end_list_ids;
  static vector<long> synapse_start_list_ids, synapse_end_list_ids;
  static vector<long> pre_neuron, post_neuron;
  static vector<string> var_list_ids;
  static state_type var_vals;
  static state_type get_variables() {
    return var_vals;
  }
  static long get_index(long id, string variable, int mode) {
    long startindex, endindex;
    switch(mode) {
      case NEURON: {
        startindex = neuron_start_list_ids.at(id);
        endindex = neuron_end_list_ids.at(id);
        break;
      }
      case SYNAPSE: {
        startindex = synapse_start_list_ids.at(id);
        endindex = synapse_end_list_ids.at(id);
        break;
      }
    }
    for(long iter=startindex;iter<endindex;++iter) {
      if(variable.compare(var_list_ids[iter]) == 0) {
        return iter;
      }
    }
    cout<<"FATAL ERROR: nnet::get methods supplied with malformed / incorrect arguments."
        <<"Arguments were: id= "<<id<<" variable= "<<variable<<" mode= "
        <<mode<<" (1 - NEURON, 2 - SYNAPSE, Other - UNKNOWN)"<<endl<<"Exiting.";
    exit(0);
  }
  static double get_value(long index) {
    return var_vals.at(index);
  }
  static double get(long id, string variable, int mode) {
    return get_value(get_index(id, variable, mode));
  }
  static double get_sum(long neuron_id, string variable, int mode=SYNAPSE) {
   double sum = 0;
    for(long index = 0; index < post_neuron.size(); ++index) {
      if(neuron_id == post_neuron.at(index)) {
        sum += get(index, variable, SYNAPSE);
      }
    }
    return sum;
  }
  static double get_diff(long synapse_id, string first_variable, string second_variable, int mode=NEURON) {
    return get(pre_neuron.at(synapse_id)-1,first_variable,NEURON) -
        get(pre_neuron.at(synapse_id)-1,second_variable,NEURON);
  }
  static vector<long> get_indices(long neuron_id, string variable, int mode=SYNAPSE) {
    vector<long> indices = {};
    for(long index = 0; index < post_neuron.size(); ++index) {
      if(neuron_id == post_neuron.at(index)) {
        indices.push_back(get_index(index, variable, SYNAPSE));
      }
    }
    return indices;
  }
  static double get_count(long neuron_id, string variable, int mode=SYNAPSE) {
    long count = 0;
    for(long index = 0; index < post_neuron.size(); ++index) {
      if(neuron_id == post_neuron.at(index)) {
        if(get_index(index, variable, SYNAPSE) >= 0) {
          ++count;
        }
      }
    }
    return count;
  }
  static void read(string neuron_file, string synapse_file)
  {
    string str = "", c_var = "";
    long ncount = 0;
    ifstream neuron_stream(neuron_file), synapse_stream(synapse_file);
    assert(neuron_stream.is_open()); // safety check
    assert(synapse_stream.is_open()); // safety check

    while(getline(neuron_stream,str) > 0) {
      neuron_start_list_ids.push_back(ncount);
      for (int str_index=0; str_index<str.length();++str_index) {
        c_var = "";
        while(str_index<str.length() && str.at(str_index)!=':') {
          c_var+=str.at(str_index);
          ++str_index;
        }
        var_list_ids.push_back(c_var);
        c_var = "";
        ++str_index;
        while(str_index<str.length() && str.at(str_index)!=',') {
          c_var+=str.at(str_index);
          ++str_index;
        }
        size_t sz;
        var_vals.push_back(stod(c_var,&sz));
        ++ncount;
      }
      neuron_end_list_ids.push_back(ncount);
    }
    bool pre=false, post=false;
    while(getline(synapse_stream,str) > 0){
      synapse_start_list_ids.push_back(ncount);
      for (int str_index=0; str_index<str.length(); ++str_index) {
        c_var = "";
        while(str_index<str.length() && str.at(str_index)!=':') {
          c_var+=str.at(str_index);
          ++str_index;
        }
        if(c_var.compare("pre")==0){
          pre=true; post=false;
        }
        else if(c_var.compare("post")==0){
          pre=false; post=true;
        }
        else {
          var_list_ids.push_back(c_var);
          pre=false; post=false;
        }      
        c_var = "";
        ++str_index;
        while(str_index<str.length() && str.at(str_index)!=',') {
          c_var+=str.at(str_index);
          ++str_index;
        }
        size_t sz;
        if(pre==true) {
          pre_neuron.push_back(stoi(c_var,&sz));
        }
        else if(post==true){
          post_neuron.push_back(stoi(c_var,&sz));
        }
        else {
          var_vals.push_back(stod(c_var,&sz));
          ++ncount;
        }  
      }
      synapse_end_list_ids.push_back(ncount);
    }
    neuron_stream.close();
    synapse_stream.close();
  }
  void operator()(const state_type &variables, state_type &dxdt, const double time);
}nnet;

vector<long> nnet::neuron_start_list_ids;
vector<long> nnet::neuron_end_list_ids;
vector<long> nnet::synapse_start_list_ids;
vector<long> nnet::synapse_end_list_ids;
vector<long> nnet::pre_neuron;
vector<long> nnet::post_neuron;
vector<string> nnet::var_list_ids;
state_type nnet::var_vals;

struct configuration {
  ofstream &stream;
  configuration(ofstream &file): stream(file) {}
  void operator()(const state_type &variables, const double t) {
    assert(stream.is_open());
    stream<<t;
    for(double variable: variables){
      stream<<','<<variable;
    }
    stream<<endl;
  }
};

class value {
 private:
  long double datum;
 public:
  value() { datum = 0.0; }
  value(long double datum) { this->datum = datum; }
  long double get() const { return datum; }
  void set(long double datum) {	this->datum = datum; }
};

class current: public value {};

class voltage: public value {};

class conductance: public value {
 public:
  conductance(): value{0.0} {}
  conductance(long double conductance_value): value{conductance_value} {}
  void ode_set() {}
};

class probability_variable : public value {
 private:
  bool valid;
 public:
  probability_variable() : value{0.0} { valid = true; }
  probability_variable(long double probability_value): value{probability_value} {
    valid = (probability_value < 0 || probability_value > 1);
  }
  bool is_valid() { return valid; }
};

class na_conductance: public conductance {
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

class k_conductance: public conductance {
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

class leak_conductance: public conductance {
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

    double gna = nnet::get(index, "gna", NEURON);
    double ena = nnet::get(index, "ena", NEURON);
    double gk = nnet::get(index, "gk", NEURON);
    double ek = nnet::get(index, "ek", NEURON);
    double gl = nnet::get(index, "gl", NEURON);
    double el = nnet::get(index, "el", NEURON);
    double iext = nnet::get(index, "iext", NEURON);
  
    long multiple_esyn_count = nnet::get_count(index, "esyn", SYNAPSE);
    vector<long> g1_indices = nnet::get_indices(index, "g1", SYNAPSE);
    vector<long> esyn_indices = nnet::get_indices(index, "esyn", SYNAPSE);    
    double isyn = 0;

    for(long iterator = 0; iterator < g1_indices.size(); ++iterator) {
      isyn = isyn + variables[g1_indices.at(iterator)] * (v - variables[esyn_indices.at(iterator)]); 
    }
    if(index == 1) cout<<t<<" "<<isyn<<endl;
    // ODE
    dxdt[v_index] = -gna*pow(m,3)*h*(v-ena)-gk*pow(n,4)*(v-ek)-gl*(v-el)+iext+isyn;

    k_conductance::ode_set(variables, dxdt, t, index);
    na_conductance::ode_set(variables, dxdt, t, index);
    leak_conductance::ode_set(variables, dxdt, t, index);

    // For synapse
    long prev_v_index = nnet::get_index(index, "prev_v", NEURON);
    long spike_t_index = nnet::get_index(index, "spike_t", NEURON);
    long spike_flag_index = nnet::get_index(index, "spike_flag", NEURON);
    long prev_spike_t_index = nnet::get_index(index, "prev_spike_t", NEURON);
    
    double prev_v = variables[prev_v_index];
    int spike_flag = variables[spike_flag_index];
    double spike_t = variables[spike_t_index];
    double prev_spike_t = variables[prev_spike_t_index];

    double thresh = nnet::get(index, "thresh", NEURON);
    
    if(prev_v > thresh && v > thresh && prev_v < v && spike_flag < 1) {
      if(prev_spike_t < -9998.0) dxdt[prev_spike_t_index] = spike_t;
      dxdt[spike_t_index] = t;
      spike_flag++;
    }
    else {
      spike_flag = 0;
    }
    dxdt[prev_v_index] = v;
    dxdt[spike_flag_index] = spike_flag;
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

    //double delay = nnet::get(index, "delay", SYNAPSE);
    //double spike_time_diff = nnet::get_diff(index, "v", "prev_v", NEURON);
    double xt = 1;//(spike_time_diff >= delay) ? 1 : 0;

    dxdt[g1_index] = g2;
    dxdt[g2_index] = -((tau1+tau2)/(tau1*tau2))*g2-g1+gsyn*xt; 
  }
};

void nnet::operator()(const state_type &variables, state_type &dxdt,
                       const double time) {
  long network_size = neuron_start_list_ids.size();
  long synapse_count = synapse_start_list_ids.size();

  for(long neuron_index = 0; neuron_index < network_size; ++neuron_index) {
    hodgkin_huxley_neuron::ode_set(variables, dxdt, time, neuron_index);
  }
  for (long synapse_index = 0; synapse_index < synapse_count; ++synapse_index) {
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
