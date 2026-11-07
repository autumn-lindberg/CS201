#!/bin/sh

i=1
while [ "$i" -le 1000 ]; do
	
	as test.s -o test.o
	as nopsled.s -o nopsled.o
	ld -z execstack -no-pie test.o nopsled.o -o test
	# input must be byte aligned
	# 4 for buffer size
	# 8 for rbp
	# 8 for rip
  ./test abcdefghijklmnopqrst

	status=$?

	if [ "$status" -eq 139 ]; then
		echo "Segmentation fault detected"
	fi

	echo "Exit status: $status"
  sleep .1

  i=$((i + 1))

done

