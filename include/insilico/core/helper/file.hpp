/*
 core/helper/file.hpp - String manipulation helper header and source

 Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

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

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_FILE_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_FILE_HPP

#include <iostream>
#include <fstream>
#include <string>

namespace insilico {

// check if the file is present and is permitted to open
bool file_check(const std::ifstream &stream, const std::string &filename) {
  if((!filename.empty()) && stream.is_open() == false) {
    std::cerr<<"[insilico::core::helper::file] Input file "<< filename
             <<" that does not exist or cannot be opened.\n";
    return false;
  }
  return true;
}

} // namespace insilico

#endif
