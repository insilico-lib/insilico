#!/bin/sh
#
# Script for continuous display of simulation output through Gnuplot
#

if [ $# -eq 0 ]
then
   echo "Error: File name missing form command line argument."
   echo "Syntax : $0 filename"
   echo "Use to continuous display of simulation output through Gnuplot"
exit 1
fi

gnuplot -persist -e "set term jpeg;
                     set key autotitle columnhead;
                     set output \"output_file_v.jpeg\";
                     set datafile separator \",\";
                     plot \""$1"\" using 1:2 with lines;"

open output_file_v.jpeg
