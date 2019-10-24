# Author: John David Lin

#!/bin/bash

# Remove files larger than a certain size

read -p "Enter file size in MB: " SIZE

if ! [[ "$SIZE" =~ ^[0-9]+$ ]]
then 
	echo "Integers only"
else
	echo "Removing files larger than $SIZE MB: "
	SIZE="+${SIZE}M"

	find . -size $SIZE -exec ls -lh {} \;
	find . -size $SIZE -exec ls {} \; > largeFiles.txt
	find . -size $SIZE -exec rm {} \;
fi




