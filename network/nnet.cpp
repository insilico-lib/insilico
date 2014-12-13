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
#ifdef MAP
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
#ifdef MAP  
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
  for(vector<long>::size_type index = 0; index < post_neuron.size(); ++index) {
    if(neuron_id == post_neuron.at(index)) {
      indices.push_back(synapse_index(index, variable));
    }
  }
  return indices;
}

void neuronal_network::read(string neuron_file, string synapse_file) {
  string str = "", c_var = "";
#ifdef MAP
  string key;
  long ntrack = 0;
  long strack = 0;
#endif
  long ncount = 0;
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
      var_list_ids.push_back(c_var);
#ifdef MAP
      key = "n" + to_string(ntrack) + c_var;
#endif
      c_var = "";
      ++str_index;
      while(str_index<str.length() && str.at(str_index)!=',') {
        c_var+=str.at(str_index);
        ++str_index;
      }
      size_t sz;
      var_vals.push_back(stod(c_var,&sz));
#ifdef MAP
      index_map[key] = ncount;
      value_map[key] = stod(c_var,&sz);
#endif
      ++ncount;
    }
#ifdef MAP
    ntrack+=1;
#endif
    neuron_end_list_ids.push_back(ncount);
  }
  neuron_stream.close();

  if(synapse_file.length()!=0) {
    ifstream synapse_stream(synapse_file);
    if(synapse_stream.is_open() == false) {
      cout<<"Runtime Failure\nSimulation Exception: nnet::read supplied with file ("<< synapse_file <<") that does not exists."<<endl;
      exit(0);
    }

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
#ifdef MAP
        key = "s" + to_string(strack) + c_var;
#endif
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
#ifdef MAP
        index_map[key] = ncount;
        value_map[key] = stod(c_var,&sz);
#endif
        ++ncount;
      }
#ifdef MAP
      strack+=1;
#endif
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

std::unordered_map<std::string, long> nnet::index_map;
std::unordered_map<string, long double> nnet::value_map;
std::vector<long> nnet::neuron_start_list_ids;
std::vector<long> nnet::neuron_end_list_ids;
std::vector<long> nnet::synapse_start_list_ids;
std::vector<long> nnet::synapse_end_list_ids;
std::vector<long> nnet::pre_neuron;
std::vector<long> nnet::post_neuron;
std::vector<std::string> nnet::var_list_ids;
state_type nnet::var_vals;
