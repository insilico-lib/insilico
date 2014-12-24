/*
 core/configuration.hpp - insilico Configuration header and source

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

#ifndef INCLUDED_CONFIGURATION_HPP
#define INCLUDED_CONFIGURATION_HPP

#include "core/engine.hpp"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace insilico;
using namespace std;

namespace insilico {

class configuration {
 public:

  static ofstream outstream;

  static void initialize(int argc, char** argv) {
    if(argc != 2) {
      cout<<"Usage: "<<argv[0]<<" <outputfile>.dat"<<endl;
      exit(0);
    }
    outstream.open(argv[1], ios::out);
  } // function initialize

  static void finalize() {
    cout<<endl<<"Done. Cleaning up."<<endl;
    outstream.close();
  } // function finalize

  static void read(string neuron_file, string synapse_file="") {
    string str="", c_var="", key="";
    int ntrack = 0, strack = 0, ncount = 0, dxdt_count = 0;
    bool dxdt_read = false;

    ifstream neuron_stream(neuron_file);
    if(neuron_stream.is_open() == false) {
      cout<<"Runtime Failure"<<endl
          <<"Simulation Exception: nnet::read supplied with file ("<< neuron_file <<") that does not exists."
          <<endl;
      exit(0);
    }

    while(getline(neuron_stream,str) > 0) {
      if(str.length()==0) continue;
      engine::neuron_start_list_ids.push_back(ncount);
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
          engine::var_list_ids.push_back(c_var);
        }
        key = "n" + to_string(ntrack) + c_var;
        c_var = "";
        ++str_index;
        while(str_index<str.length() && str.at(str_index)!=',') {
          c_var+=str.at(str_index);
          ++str_index;
        }
        size_t sz;
        engine::value_map[key] = stod(c_var,&sz);
        if(dxdt_read == true) {
          dxdt_count = stod(c_var,&sz);
          dxdt_read = false;
        }
        else if(dxdt_count > 0){
          engine::index_map[key] = ncount;
          engine::var_vals.push_back(stod(c_var,&sz));
          ++ncount;
          --dxdt_count;
        }
      }
      ntrack+=1;
      engine::neuron_end_list_ids.push_back(ncount);
    }
    neuron_stream.close();

    if(synapse_file.length()!=0) {
      ifstream synapse_stream(synapse_file);
      bool pre=false, post=false;
      if(synapse_stream.is_open() == false) {
        cout<<"Runtime Failure"<<endl
            <<"Simulation Exception: nnet::read supplied with file ("<< synapse_file <<") that does not exists."
            <<endl;
        exit(0);
      }

      while(getline(synapse_stream,str) > 0){
        if(str.length()==0) continue;
        engine::synapse_start_list_ids.push_back(ncount);
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
            engine::var_list_ids.push_back(c_var);
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
            engine::pre_neuron.push_back(stoi(c_var,&sz));
          }
          else if(post==true) {
            engine::post_neuron.push_back(stoi(c_var,&sz));
          }
          engine::value_map[key] = stod(c_var,&sz);
          if(dxdt_read == true) {
            dxdt_count = stod(c_var,&sz);
            dxdt_read = false;
          }
          else if(dxdt_count > 0) {
            engine::index_map[key] = ncount;
            engine::var_vals.push_back(stod(c_var,&sz));
            ++ncount;
            --dxdt_count;
          }
        }
        strack+=1;
        engine::synapse_end_list_ids.push_back(ncount);
      }
      synapse_stream.close();
    }
    engine::populate_pre_synaptic_lists();
  } // function read

  struct observer {
    ofstream &stream;

    observer(ofstream &stream_): stream(stream_) {}

    void operator()(const state_type &variables, const double t);
  }; // struct observer

}; // class configuration

// static member definitions
ofstream configuration::outstream;

} // namespace insilico

#endif
