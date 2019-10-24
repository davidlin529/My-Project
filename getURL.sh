# Author: John David Lin

#!/bin/bash

# Download search results in xml file

read -p "Enter query (Sample: term=GSE[ETYP]+AND+\"Th17\"[Title]): " QUERY

URL="https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?db=gds&$QUERY&retmax=5000"

wget -O esearch.xml "$URL"

# Extract UID
grep '<Id>[0-9]' esearch.xml | grep -o '[0-9]*' > acession.txt 
echo -n "acession numbers recorded: "
wc -l acession.txt

# Convert UID into URL
PREFIX=ftp://ftp.ncbi.nlm.nih.gov/geo/series/

while read i
do
	ACESSION=$(echo $i | sed -E 's/^[0-9][0]*/GSE/')
	SERIES=$(echo $ACESSION | sed -E 's/[0-9]{3}$/nnn/')
	echo $i | awk -v prefix=$PREFIX -v series=$SERIES -v acession=$ACESSION '{print prefix series "/" acession}'
done < acession.txt > url.txt

rm acession.txt
