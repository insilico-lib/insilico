#!/bin/bash
echo "" > machinefile 

for (( i=1; i<=69; i++  ))
do
  node="node"
  if [ $i -le 9 ] 
  then
    node="node0";
  fi
  node=$node$i  

  echo $node" slots=1" >> machinefile
done
