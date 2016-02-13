#!/bin/bash

for i in `seq 1 10`; do
	./build/random_output raw seed_device 256 > "tempout$i"
done

for i in `seq 2 10`; do
	if cmp "tempout1" "tempout$i" -s; then
		echo "FAIL"
	fi
done