#!/bin/bash
rm plot.dat

writedata() {
    for i in {1..20}; do
        echo -e $i"\t"$((i*i)) >> plot.dat
        sleep 1
        echo "done"
    done
}

writedata &
sleep 1
gnuplot liveplot.gnu
