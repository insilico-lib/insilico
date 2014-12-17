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

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <network/nnet.hpp>

//#define MAP
#define MREAD

using namespace std;

char key[128];

state_type neuronal_network::get_variables() {
  return var_vals;
}

long neuronal_network::neuron_index(long id, string variable) {
#ifdef MAP
  try {
    sprintf(key, "n%ld%s", id, variable.c_str());
    return index_map[key];
  }
  catch(const char* msg) {
    cout<<"Runtime Failure\nSimulator Exception: nnet::neuron_index method supplied with incorrect arguments."
        <<"Arguments were: [neuron_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    cout<<"C++ Exception"<<msg;
  }
  exit(0);
#else
  long startindex = 0, endindex = 0;
  startindex = neuron_start_list_ids.at(id);
  endindex = neuron_end_list_ids.at(id);
  
  for(long iter=startindex;iter<endindex;++iter) {
    if(variable.compare(var_list_ids[iter]) == 0) {
      return iter;
    }
  }
  cout<<"Runtime Failure\nSimulator Exception: nnet::neuron_index method supplied with incorrect arguments."
      <<"Arguments were: [neuron_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
  exit(0);
#endif
}

long double neuronal_network::neuron_value(long id, string variable) {
#ifdef MREAD
  try {
    sprintf(key, "n%ld%s", id, variable.c_str());
    return value_map[key];
  }
  catch(const char* msg) {
    cout<<"Runtime Failure\nSimulator Exception: nnet::neuron_value method supplied with incorrect arguments."
        <<"Arguments were: [neuron_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
  }
  exit(0);
#else
  return get_value(get_index(id, variable, NEURON));
#endif
}

long neuronal_network::synapse_index(long id, string variable) {
#ifdef MAP
  try {
    sprintf(key, "s%ld%s", id, variable.c_str());
    return index_map[key];
  }
  catch(const char* msg) {
    cout<<"Runtime Failure\nSimulator Exception: nnet::synapse_index method supplied with incorrect arguments."
        <<"Arguments were: [synapse_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    cout<<"C++ Exception: "<<msg;
  }
  exit(0);
#else
  long startindex = 0, endindex = 0;
  startindex = synapse_start_list_ids.at(id);
  endindex = synapse_end_list_ids.at(id);

  for(long iter=startindex;iter<endindex;++iter) {
    if(variable.compare(var_list_ids[iter]) == 0) {
      return iter;
    }
  }
  cout<<"Runtime Failure\nSimulator Exception: nnet::synapse_index method supplied with incorrect arguments."
      <<"Arguments were: [synapse_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
  exit(0);
#endif
}

long double neuronal_network::synapse_value(long id, string variable) {
#ifdef MREAD
  try {
    sprintf(key, "s%ld%s", id, variable.c_str());
    return value_map[key];
  }
  catch(const char* msg) {
    cout<<"Runtime Failure\nSimulator Exception: nnet::synapse_value method supplied with incorrect arguments."
        <<"Arguments were: [synapse_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    cout<<"C++ Exception: "<<msg;
  }
  exit(0);
#else
  return get_value(get_index(id, variable, SYNAPSE));
#endif
}

double neuronal_network::get_value(long index) {
  return var_vals.at(index);
}

vector<long> neuronal_network::get_indices(string variable) {
  vector<long> indices;
  for(vector<long>::size_type index = 0; index < var_list_ids.size(); ++index) {
    if(variable.compare(var_list_ids[index]) == 0) {
      indices.push_back(index);
    }
  }
  if(indices.size() == 0) {
    cout<<"FATAL ERROR: nnet::get methods supplied with malformed / incorrect arguments."
        <<"Searching for all indices of variable = "<<variable<<endl<<"Exiting."<<endl;
    exit(0);
  }
  return indices;
}

vector<long> neuronal_network::get_pre_neuron_indices(long neuron_id, string variable) {
  vector<long> indices;
  for(vector<long>::size_type index = 0; index < pre_neuron_lists[neuron_id].size(); ++index) {
    indices.push_back(synapse_index(pre_neuron_lists[neuron_id][index], variable));
  }
  return indices;
}

vector<long> neuronal_network::get_pre_neuron_values(long neuron_id, string variable) {
  vector<long> values;
  for(vector<long>::size_type index = 0; index < pre_neuron_lists[neuron_id].size(); ++index) {
    values.push_back(synapse_value(pre_neuron_lists[neuron_id][index], variable));
  }
  return values;
}

void neuronal_network::populate_pre_synaptic_lists() {
  auto max_post_neuron = *max_element(post_neuron.begin(), post_neuron.end());

  pre_neuron_lists.resize( max_post_neuron + 1 );

  for(vector<long>::size_type iterator = 0; iterator < post_neuron.size(); ++iterator) {
    pre_neuron_lists[ post_neuron[iterator] ].push_back( iterator );
  }
}

void neuronal_network::read(string neuron_file, string synapse_file) {
  string str="", c_var="", key="";
  long ntrack = 0, strack = 0;
  long ncount = 0;
  bool dxdt_read = false;
  long dxdt_count = 0;

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

long neuronal_network::neuron_count() {
  return neuron_start_list_ids.size();
}

long neuronal_network::synapse_count() {
  return synapse_start_list_ids.size();
}

unordered_map<std::string, long> nnet::index_map;
unordered_map<string, long double> nnet::value_map;
vector<long> nnet::neuron_start_list_ids;
vector<long> nnet::neuron_end_list_ids;
vector<long> nnet::synapse_start_list_ids;
vector<long> nnet::synapse_end_list_ids;
vector<long> nnet::pre_neuron;
vector<long> nnet::post_neuron;
vector< vector<long> > nnet::pre_neuron_lists;
vector<std::string> nnet::var_list_ids;
state_type nnet::var_vals;
