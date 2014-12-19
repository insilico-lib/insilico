/*
 network/nnet.cpp - nsim's Neuronal Network API source

 Copyright (C) 2014 Collins Assisi, Collins Assisi Lab, IISER, Pune
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

#include "network/nnet.hpp"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#define MREAD

using namespace std;

char key[128];

state_type neuronal_network::get_variables() {
  return var_vals;
}

int neuronal_network::neuron_index(int id, string variable) {
#ifdef MREAD
  try {
    sprintf(key, "n%d%s", id, variable.c_str());
    return index_map[key];
  }
  catch(const char* msg) {
    cout<<"Runtime Failure\nSimulator Exception: nnet::neuron_index method supplied with incorrect arguments."
        <<"Arguments were: [neuron_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    cout<<"C++ Exception"<<msg;
  }
  exit(0);
#else
  int startindex = neuron_start_list_ids.at(id);
  int endindex = neuron_end_list_ids.at(id);
  for(int iter=startindex;iter<endindex;++iter) {
    if(variable.compare(var_list_ids[iter]) == 0) {
      return iter;
    }
  }
  cout<<"Runtime Failure\nSimulator Exception: nnet::neuron_index method supplied with incorrect arguments."
      <<"Arguments were: [neuron_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
  exit(0);
#endif
}

float neuronal_network::neuron_value(int id, string variable) {
  try {
    sprintf(key, "n%d%s", id, variable.c_str());
    return value_map[key];
  }
  catch(const char* msg) {
    cout<<"Runtime Failure\nSimulator Exception: nnet::neuron_value method supplied with incorrect arguments."
        <<"Arguments were: [neuron_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
  }
  exit(0);
}

int neuronal_network::synapse_index(int id, string variable) {
#ifdef MAP
  try {
    sprintf(key, "s%d%s", id, variable.c_str());
    return index_map[key];
  }
  catch(const char* msg) {
    cout<<"Runtime Failure\nSimulator Exception: nnet::synapse_index method supplied with incorrect arguments."
        <<"Arguments were: [synapse_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    cout<<"C++ Exception: "<<msg;
  }
  exit(0);
#else
  int startindex = synapse_start_list_ids.at(id);
  int endindex = synapse_end_list_ids.at(id);
  for(int iter=startindex;iter<endindex;++iter) {
    if(variable.compare(var_list_ids[iter]) == 0) {
      return iter;
    }
  }
  cout<<"Runtime Failure\nSimulator Exception: nnet::synapse_index method supplied with incorrect arguments."
      <<"Arguments were: [synapse_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
  exit(0);
#endif
}

float neuronal_network::synapse_value(int id, string variable) {
  try {
    sprintf(key, "s%d%s", id, variable.c_str());
    return value_map[key];
  }
  catch(const char* msg) {
    cout<<"Runtime Failure\nSimulator Exception: nnet::synapse_value method supplied with incorrect arguments."
        <<"Arguments were: [synapse_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    cout<<"C++ Exception: "<<msg;
  }
  exit(0);
}

vector<int> neuronal_network::get_indices(string variable) {
  vector<int> indices;
  int total_size = neuron_count();
  for(int index = 0; index < total_size; ++index) {
    indices.push_back(neuron_index(index,variable));
  }
  if(indices.empty()) {
    cout<<"FATAL ERROR: nnet::get methods supplied with malformed / incorrect arguments."
        <<"Searching for all indices of variable = "<<variable<<endl<<"Exiting."<<endl;
    exit(0);
  }
  return indices;
}

vector<int> neuronal_network::get_pre_neuron_indices(int neuron_id, string variable) {
  vector<int> indices;
  for(vector<int>::size_type index = 0; index < pre_synaptic_lists[neuron_id].size(); ++index) {
    indices.push_back(synapse_index(pre_synaptic_lists[neuron_id][index], variable));
  }
  return indices;
}

vector<int> neuronal_network::get_pre_neuron_values(int neuron_id, string variable) {
  vector<int> values;
  for(vector<int>::size_type index = 0; index < pre_synaptic_lists[neuron_id].size(); ++index) {
    values.push_back(synapse_value(pre_synaptic_lists[neuron_id][index], variable));
  }
  return values;
}

void neuronal_network::populate_pre_synaptic_lists() {
  pre_synaptic_lists.resize( *max_element(post_neuron.begin(), post_neuron.end()) + 1 );
  for(vector<int>::size_type iterator = 0; iterator < post_neuron.size(); ++iterator) {
    pre_synaptic_lists[ post_neuron[iterator] ].push_back( iterator );
  }
}

void neuronal_network::read(string neuron_file, string synapse_file) {
  string str="", c_var="", key="";
  int ntrack = 0, strack = 0, ncount = 0, dxdt_count = 0;
  bool dxdt_read = false;

  ifstream neuron_stream(neuron_file);
  if(neuron_stream.is_open() == false) {
    cout<<"Runtime Failure\nSimulation Exception: nnet::read supplied with file ("<< neuron_file <<") that does not exists."<<endl;
    exit(0);
  }

  while(getline(neuron_stream,str) > 0) {
    if(str.length()==0) continue;
    neuron_start_list_ids.push_back(ncount);
    for (size_t str_index=0; str_index<str.length();++str_index) {
      c_var = "";
      while(str_index<str.length() && str.at(str_index)!=':') {
        c_var+=str.at(str_index);
        ++str_index;
      }
      if(c_var.compare("dxdt")==0) {
        dxdt_read = true;
      }
      else if(dxdt_count > 0) {
        var_list_ids.push_back(c_var);
      }
      key = "n" + to_string(ntrack) + c_var;
      c_var = "";
      ++str_index;
      while(str_index<str.length() && str.at(str_index)!=',') {
        c_var+=str.at(str_index);
        ++str_index;
      }
      size_t sz;
      value_map[key] = stod(c_var,&sz);
      if(dxdt_read == true) {
        dxdt_count = stod(c_var,&sz);
        dxdt_read = false;
      }
      else if(dxdt_count > 0){
        index_map[key] = ncount;
        var_vals.push_back(stod(c_var,&sz));
        ++ncount;
        --dxdt_count;
      }
    }
    ntrack+=1;
    neuron_end_list_ids.push_back(ncount);
  }
  neuron_stream.close();

  if(synapse_file.length()!=0) {
    ifstream synapse_stream(synapse_file);
    bool pre=false, post=false;
    if(synapse_stream.is_open() == false) {
      cout<<"Runtime Failure\nSimulation Exception: nnet::read supplied with file ("<< synapse_file <<") that does not exists."<<endl;
      exit(0);
    }

    while(getline(synapse_stream,str) > 0){
      if(str.length()==0) continue;
      synapse_start_list_ids.push_back(ncount);
      for (size_t str_index=0; str_index<str.length(); ++str_index) {
        c_var = "";
        while(str_index<str.length() && str.at(str_index)!=':') {
          c_var+=str.at(str_index);
          ++str_index;
        }
        if(c_var.compare("dxdt") == 0) {
          dxdt_read = true;
        }
        else if(dxdt_count > 0) {
          var_list_ids.push_back(c_var);
        }
        key = "s" + to_string(strack) + c_var;
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
        else if(post==true) {
          post_neuron.push_back(stoi(c_var,&sz));
        }
        value_map[key] = stod(c_var,&sz);
        if(dxdt_read == true) {
          dxdt_count = stod(c_var,&sz);
          dxdt_read = false;
        }
        else if(dxdt_count > 0) {
          index_map[key] = ncount;
          var_vals.push_back(stod(c_var,&sz));
          ++ncount;
          --dxdt_count;
        }
      }
      strack+=1;
      synapse_end_list_ids.push_back(ncount);
    }
    synapse_stream.close();
  }
  populate_pre_synaptic_lists();
}

int neuronal_network::neuron_count() {
  return neuron_start_list_ids.size();
}

int neuronal_network::synapse_count() {
  return synapse_start_list_ids.size();
}

unordered_map<std::string, int> nnet::index_map;
unordered_map<string, float> nnet::value_map;
vector<int> nnet::neuron_start_list_ids;
vector<int> nnet::neuron_end_list_ids;
vector<int> nnet::synapse_start_list_ids;
vector<int> nnet::synapse_end_list_ids;
vector<int> nnet::pre_neuron;
vector<int> nnet::post_neuron;
vector< vector<int> > nnet::pre_synaptic_lists;
vector<std::string> nnet::var_list_ids;
state_type nnet::var_vals;
