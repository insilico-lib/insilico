#
#   Makefile - nsim project source compilation
#
#   Copyright (C) 2014 Collins Assisi
#   Copyright (C) 2014 Pranav Kulkarni
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Target excutables
TARGET   = main.out

# Compile directory
OBJDIR	 = nsim/lib

# Compiler to use
CC       = g++ -c

# Flags to specify the compilation startegy
CFLAGS   = -std=c++11 -Wall -I.

# Linker to use
LINKER   = g++ -o

# Flags to specify the linker startegy
LFLAGS   = -Wall

# All project sources that need to be compiled
SOURCES  := main.cpp nsim/network/nnet.cpp

# Flags to specify the inclusion of external libraries
INCLUDES := nsim/network/nnet.hpp

# Target objects
OBJECTS  := main.o nnet.o

# Cleaning forcefully
mv       = mv -f

clean: $(TARGET)
	mkdir -p $(OBJDIR)
	$(mv) $(OBJECTS) $(OBJDIR)

$(TARGET): objects
	$(LINKER) $(TARGET) $(LFLAGS) $(OBJECTS)

objects: $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) $(SOURCES)
