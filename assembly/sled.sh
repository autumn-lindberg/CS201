#!/bin/sh

i=1
while [ "$i" -le 1000 ]; do
  ./test asdfgh

	status=$?

	if [ "$status" -eq 139 ]; then
		echo "Segmentation fault detected"
	fi

	echo "Exit status: $status"
  sleep .25

  i=$((i + 1))

done

