#!/bin/bash

mkdir solExec

echo "--------------------"
echo "Handover 20_05_270001"
echo "--------------------"
./main instances/Handover/20_5_270001 ./solExec/20_5.txt 1 0.3
echo "--------------------"
echo "Handover 20_10_270001"
echo "--------------------"
./main instances/Handover/20_10_270001 ./solExec/20_10.txt 1 0.3
echo "--------------------"
echo "Handover 30_05_270003"
echo "--------------------"
./main instances/Handover/30_5_270003 ./solExec/30_5.txt 1 0.3

echo "--------------------"
echo "Sparse82_01"
echo "--------------------"
./main instances/Sparse82/Sparse82_01.txt ./solExec/82_01.txt 0 0.3
echo "--------------------"
echo "Sparse82_02"
echo "--------------------"
./main instances/Sparse82/Sparse82_02.txt ./solExec/82_02.txt 0 0.3
echo "--------------------"
echo "Sparse82_03"
echo "--------------------"
./main instances/Sparse82/Sparse82_03.txt ./solExec/82_03.txt 0 0.3

echo "--------------------"
echo "RanReal240_01"
echo "--------------------"
./main instances/RanReal240/RanReal240_01.txt ./solExec/240_01.txt 0 0.3
echo "--------------------"
echo "RanReal240_04"
echo "--------------------"
./main instances/RanReal240/RanReal240_04.txt ./solExec/240_04.txt 0 0.3
echo "--------------------"
echo "RanReal240_07"
echo "--------------------"
./main instances/RanReal240/RanReal240_07.txt ./solExec/240_07.txt 0 0.3

echo "--------------------"
echo "RanReal480_01"
echo "--------------------"
./main instances/RanReal480/RanReal480_01.txt ./solExec/480_01.txt 0 0.3
echo "--------------------"
echo "RanReal480_06"
echo "--------------------"
./main instances/RanReal480/RanReal480_06.txt ./solExec/480_06.txt 0 0.3
echo "--------------------"
echo "RanReal480_12"
echo "--------------------"
./main instances/RanReal480/RanReal480_12.txt ./solExec/480_12.txt 0 0.3

echo "DONE"
