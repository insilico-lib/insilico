#   Makefile - nsim project source compilation
#
#   Copyright (C) 2014 Pranav Kulkarni
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Compiler to use
CC=g++

# Flags to specify the compilation startegy
CFLAGS=-c -O3 -std=c++11

# Flags to specify the inclusion of external libraries
# INLCUDES=-I/usr/local/include/boost_1_56_0
INLCUDES=-IC:\MinGW\lib\gcc\mingw32\4.8.1\include\boost_1_56_0

# All project sources that need to be compiled
SOURCES=main.cpp

# Target objects
OBJECTS=$(SOURCES:.cpp=.o)

# Target excutables
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(INLCUDES) $(CFLAGS) $< -o $@
