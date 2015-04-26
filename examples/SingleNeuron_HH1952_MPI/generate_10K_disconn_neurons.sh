#!/bin/bash

neuron="dxdt : 4, v : -10, m : 0, n : 0, h : 0, I_Ext : 10;"

for (( i=0; i<10000; ++i ))
do
    echo -e $neuron
done
