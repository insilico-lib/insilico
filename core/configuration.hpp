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
#include <functional>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace insilico;
using namespace std;

namespace insilico {

class configuration {
 public:

  static ofstream outstream;

  // trim from start
  static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  } // function ltrim

  // trim from end
  static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
  } // function rtrim

  // trim from both ends
  static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
  } // function trim

  // convert string literal to double precision floating point number
  static inline double string_to_double(string strnum) {
    size_t sz;
    double value;
    try {
      value = stod(strnum, &sz);
    }
    catch(const std::exception& e) {
      cout<<"[insilico::configuration] Simulation Exception: "
          <<"supplied with file that contains improper value: "<< strnum <<endl;
      exit(0);
    }
    return value;
  } // function string_to_double

  static void file_check(ifstream& stream, string& filename) {
    if(stream.is_open() == false) {
      cout<<"[insilico::configuration] Simulation Exception: insilico::configuration::initialize"
          <<" supplied with file ("<< filename <<") that does not exist."<<endl;
      exit(0);
    }
  }

  static void initialize(int argc, char** argv) {
    if(argc < 3 || argc > 4) {
      cout<<"[insilico::configuration::initialize] USAGE: "<<argv[0]
          <<" <output_file>.dat <neuron_file>.conf [<synapse_file>.conf]"<<endl;
      exit(0);
    }
    cout<<endl<<"[insilico/configuration/initialize] SUCCESS: Initializing with following parameters:"<<endl
        <<"Output file: "<<argv[1]<<endl<<"Neuron file: "<<argv[2]<<endl;
    outstream.open(argv[1], ios::out);
    string neuron_file(argv[2]);
    if(argc == 4) {
      cout<<"Synapse file: "<<argv[3]<<endl;
      string synapse_file(argv[3]);
      read(neuron_file, synapse_file);
    }
    else {
      read(neuron_file);
    }
  } // function initialize

  static void finalize() {
    cout<<"[insilico::configuration::finalize] SUCCESS: Simulation complete."<<endl;
    outstream.close();
  } // function finalize

  // read the input files - neuron_file and synapse_file
  static void read(string neuron_file, string synapse_file="") {
    int ntrack = 0, strack = 0, ncount = 0, dxdt_count = 0;
    char linedelim = ';', worddelim = ',', pairdelim = ':';
    string part, first_item, key;
    double second_item;

    ifstream neuron_stream(neuron_file);
    file_check(neuron_stream, neuron_file);

    while(getline(neuron_stream, part, linedelim)) {
      stringstream l(part);
      if((trim(part)).length() > 0) {
        engine::neuron_start_list_ids.push_back(ncount);
        while(getline(l, part, worddelim)) {
          if((trim(part)).length() > 0) {
            stringstream k(part);
            getline(k, part, pairdelim); first_item = trim(part);
            getline(k, part, pairdelim); second_item = string_to_double(trim(part));

            key = "n" + to_string(ntrack) + first_item;
            if(first_item.compare("dxdt") == 0) {
              dxdt_count = (int)second_item;
            }
            else if(dxdt_count > 0) {
              engine::var_list_ids.push_back(first_item);
              engine::var_vals.push_back(second_item);
              engine::index_map[key] = ncount;
              ++ncount;
              --dxdt_count;
            }
            // universal value map
            engine::value_map[key] = second_item;
          }
        }
        ntrack+=1;
        engine::neuron_end_list_ids.push_back(ncount);
      }
    }
    neuron_stream.close();

    ifstream synapse_stream(synapse_file);
    file_check(synapse_stream, synapse_file);

    while(getline(synapse_stream, part, linedelim)) {
      stringstream l(part);
      if((trim(part)).length() > 0) {
        engine::synapse_start_list_ids.push_back(ncount);
        while(getline(l, part, worddelim)) {
          if((trim(part)).length() > 0) {
            stringstream k(part);
            getline(k, part, pairdelim); first_item = trim(part);
            getline(k, part, pairdelim); second_item = string_to_double(trim(part));

            key = "s" + to_string(strack) + first_item;
            if(first_item.compare("dxdt") == 0) {
              dxdt_count = (int)second_item;
            }
            else if(first_item.compare("pre") == 0) {
              engine::pre_neuron.push_back((int)second_item);
            }
            else if(first_item.compare("post") == 0) {
              engine::post_neuron.push_back((int)second_item);
            }
            else if(dxdt_count > 0) {
              engine::var_list_ids.push_back(first_item);
              engine::var_vals.push_back(second_item);
              engine::index_map[key] = ncount;
              ++ncount;
              --dxdt_count;
            }
            // universal value map
            engine::value_map[key] = second_item;
          }
        }
        strack+=1;
        engine::synapse_end_list_ids.push_back(ncount);
      }
    }
    synapse_stream.close();
    engine::populate_pre_synaptic_lists();
    cout << "[insilico/configuration/read] SUCCESS: Input file read complete."<<endl;
  } // function read

  struct observer {
    ofstream &outfile;
    observer(ofstream &stream_): outfile(stream_) {}
    void operator()(const state_type &variables, const double t);
  }; // struct observer

}; // class configuration

// static member definitions
ofstream configuration::outstream;

} // namespace insilico

#endif
