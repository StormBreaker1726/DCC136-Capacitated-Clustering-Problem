#!/bin/bash

for i in {0..9}; do
    echo "--------------------"
    echo "SOLUCAO $i"
    echo "--------------------"
    ./main instances/RanReal240/RanReal240_01.txt ./solution/240_01_$i.txt 0 0.3
    echo "--------------------"
    echo "SOLUCAO $i"
    echo "--------------------"
    ./main instances/RanReal240/RanReal240_04.txt ./solution/240_04_$i.txt 0 0.3
    echo "--------------------"
    echo "SOLUCAO $i"
    echo "--------------------"
    ./main instances/RanReal240/RanReal240_07.txt ./solution/240_07_$i.txt 0 0.3
done