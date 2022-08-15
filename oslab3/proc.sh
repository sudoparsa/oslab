#!/bin/bash
echo "Start..."
for id in $(find /proc/ -maxdepth 1 -type d -name "[0-9]*"); do
	echo $(basename $id) $(grep -n "^Name" $id/status | cut -d ":" -f3)
done
