/*
 core/configuration.hpp - insilico Configuration header and source

 Copyright (C) 2014 Collins Assisi, Collins Assisi Lab, IISER, Pune
 Copyright (C) 2014-2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

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

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_HPP

#ifdef INSILICO_MPI_ENABLE
#include "core/configuration/parallel.hpp"
#else
#include "core/configuration/serial.hpp"
#endif

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace insilico { namespace configuration {

struct observer {
  std::ofstream &outfile;
  observer(std::ofstream &stream_): outfile(stream_) {}
  void operator()(state_type &variables, const double t);
};

}} // namespace insilico::configuration

#endif
