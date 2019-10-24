# Author: John David Lin

#!/bin/bash

# wget options

#-c	--continue
#-N	--timestamping
#-r	--recursive
#-np	--no-parent
#-nH	--no-host-directories
#-a	--append-output=logfile
#-t 	--tries=number
#-w	--wait=seconds
#--waitretry=seconds
#--random-wait
#--timeout=seconds

#--spider
#-nv	--no-verbose
#--show-progress
#-O 	--output-document=file

# Create log file
rm log.txt
touch log.txt

# Download from each URL in url.txt
while read i
do
	echo "Downloading from: $i"
	wget -c -N -r -np -nH -t 3 -w 0.5 --waitretry 2 --random-wait --timeout 15 $i
	#wget -c -N -r -np -nH --spider -nv -a log.txt -t 3 -w 0.5 --waitretry 5 --random-wait --timeout 15 $i
	if [ $? -ne 0 ]
	then
		echo "Error code: $?   URL: $i" 
	fi
done < url.txt
