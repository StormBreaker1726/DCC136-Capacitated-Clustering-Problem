#!/bin/bash

for i in {0..9}; do
    echo "--------------------"
    echo "SOLUCAO $i"
    echo "--------------------"
    ./main instances/Sparse82/Sparse82_01.txt ./solution/82_01_$i.txt 1 0.3
    echo "--------------------"
    echo "SOLUCAO $i"
    echo "--------------------"
    ./main instances/Sparse82/Sparse82_02.txt ./solution/82_02_$i.txt 1 0.3
    echo "--------------------"
    echo "SOLUCAO $i"
    echo "--------------------"
    ./main instances/Sparse82/Sparse82_03.txt ./solution/82_03_$i.txt 1 0.3
done