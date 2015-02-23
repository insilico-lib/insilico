
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

gnuplot -persist -e "set term jpeg; set output \"output_file_v.jpeg\";
     set datafile separator \",\";
     plot \""$1"\" using 1:2 with lines;"

gnuplot -persist -e "set term jpeg; set output \"output_file_others.jpeg\";
       set datafile separator \",\";
       plot \""$1"\" using 1:3 with lines title \"m\",
            \""$1"\" using 1:4 with lines title \"h\",
            \""$1"\" using 1:5 with lines title \"n\";"

open output_file_v.jpeg
open output_file_others.jpeg
