#!/bin/sh
#
# Script for continuous display of simulation output through Gnuplot
#
# Copyright (c) 2015 Pranav Kulkarni
# This file is GNU GPL v3.0 licenced.
#
if [ $# -eq 0 ]
then
   echo "Error: File name missing form command line argument."
   echo "Syntax : $0 filename"
   echo "Use to continuous display of simulation output through Gnuplot"
exit 1
fi

gnuplot -persist -e "set datafile separator \",\";
     plot \""$1"\" using 1:2 with lines,
          \""$1"\" using 1:3 with lines;
     pause 2;
     replot;
     reread;"


