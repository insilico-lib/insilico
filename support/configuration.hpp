/*
 support/configuration.hpp - insilico Configuration header and source

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

#include <fstream>
#include <iostream>

using namespace std;

typedef vector<float> state_type;

namespace insilico {


class configuration {
 public:

  struct observer {
    ofstream &stream;

    observer(ofstream &stream_): stream(stream_) {}

    void operator()(const state_type &variables, const double t);
  }; // struct observer

}; // class configuration

} // namespace insilico

#endif
