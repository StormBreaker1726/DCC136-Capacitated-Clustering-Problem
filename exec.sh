#!/bin/bash

for i in {0..9}; do
    ./main instances/Handover/30_5_270003 30_5_$i.txt 1 0.3
done