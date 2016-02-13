#!/bin/bash

for i in `seq 1 10`; do
	./build/random_output ascii_newlines seed_device > "tempout$i"
done

for i in `seq 2 10`; do
	if cmp "tempout1" "tempout$i"; then
		echo "FAIL"
	fi
done