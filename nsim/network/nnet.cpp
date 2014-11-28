/*
    nnet.cpp - nsim's Neuronal Network API source

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

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <nsim/network/nnet.hpp>

using namespace std;

state_type neuronal_network::get_variables() {
  return var_vals;
}

long neuronal_network::get_index(long id, string variable, int mode) {
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
  std::cout<<"FATAL ERROR: nnet::get methods supplied with malformed / incorrect arguments."
           <<"Arguments were: [id = "<<id<<"][variable = "<<variable<<"][mode= "
           <<mode<<"] (mode can be {1 - NEURON, 2 - SYNAPSE, Other - UNKNOWN})"<<std::endl<<"Exiting."<<std::endl;
  exit(0);
}

double neuronal_network::get_value(long index) {
  return var_vals.at(index);
}

double neuronal_network::get(long id, string variable, int mode) {
  return get_value(get_index(id, variable, mode));
}

double neuronal_network::get_sum(long neuron_id, string variable, int mode) {
  double sum = 0;
  for(vector<long>::size_type index = 0; index < post_neuron.size(); ++index) {
    if(neuron_id == post_neuron.at(index)) {
      sum += get(index, variable, SYNAPSE);
    }
  }
  return sum;
}

double neuronal_network::get_diff(long synapse_id, string first_variable,
                                         string second_variable, int mode) {
  return get(pre_neuron.at(synapse_id),first_variable,NEURON) -
      get(pre_neuron.at(synapse_id),second_variable,NEURON);
}

vector<long> neuronal_network::get_indices(string variable) {
  vector<long> indices;
  for(vector<long>::size_type index = 0; index < var_list_ids.size(); ++index) {
    if(variable.compare(var_list_ids[index]) == 0) {
      indices.push_back(index);
    }
  }
  if(indices.size() == 0) {
    std::cout<<"FATAL ERROR: nnet::get methods supplied with malformed / incorrect arguments."
             <<"Searching for all indices of variable = "<<variable<<std::endl<<"Exiting."<<std::endl;
    exit(0);
  }
  return indices;
}

vector<long> neuronal_network::get_pre_neuron_indices(long neuron_id, string variable, int mode) {
  vector<long> indices;
  for(vector<long>::size_type index = 0; index < post_neuron.size(); ++index) {
    if(neuron_id == post_neuron.at(index)) {
      indices.push_back(get_index(index, variable, SYNAPSE));
    }
  }
  return indices;
}

double neuronal_network::get_count(long neuron_id, string variable, int mode) {
  long count = 0;
  for(vector<long>::size_type index = 0; index < post_neuron.size(); ++index) {
    if(neuron_id == post_neuron.at(index)) {
      if(get_index(index, variable, SYNAPSE) >= 0) {
        ++count;
      }
    }
  }
  return count;
}

void neuronal_network::read(string neuron_file, string synapse_file) {
  string str = "", c_var = "";
  long ncount = 0;
  ifstream neuron_stream(neuron_file);
  
  while(getline(neuron_stream,str) > 0) {
    if(str.length()==0) continue;
    neuron_start_list_ids.push_back(ncount);
    for (size_t str_index=0; str_index<str.length();++str_index) {
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
  neuron_stream.close();

  if(synapse_file.length()!=0) {
    ifstream synapse_stream(synapse_file);
    bool pre=false, post=false;
    while(getline(synapse_stream,str) > 0){
      if(str.length()==0) continue;
      synapse_start_list_ids.push_back(ncount);
      for (size_t str_index=0; str_index<str.length(); ++str_index) {
        c_var = "";
        while(str_index<str.length() && str.at(str_index)!=':') {
          c_var+=str.at(str_index);
          ++str_index;
        }
        var_list_ids.push_back(c_var);
        if(c_var.compare("pre")==0){
          pre=true; post=false;
        }
        else if(c_var.compare("post")==0){
          pre=false; post=true;
        }
        else {
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
        var_vals.push_back(stod(c_var,&sz));
        ++ncount;
      }
      synapse_end_list_ids.push_back(ncount);
    }
    synapse_stream.close();
  }
}

long neuronal_network::neuron_count() {
  return neuron_start_list_ids.size();
}

long neuronal_network::synapse_count() {
  return synapse_start_list_ids.size();
}

std::vector<long> nnet::neuron_start_list_ids;
std::vector<long> nnet::neuron_end_list_ids;
std::vector<long> nnet::synapse_start_list_ids;
std::vector<long> nnet::synapse_end_list_ids;
std::vector<long> nnet::pre_neuron;
std::vector<long> nnet::post_neuron;
std::vector<std::string> nnet::var_list_ids;
state_type nnet::var_vals;
