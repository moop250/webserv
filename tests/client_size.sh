#!/bin/bash

echo In client max body size testing file

URL="127.0.0.1:8001"
LARGE_DATA=$(head -c 200000 </dev/urandom | base64)

HTTP_CODE=$(curl -X POST "$LARGE_DATA" $URL)

echp $HTTP_CODE
if [ "$HTTP_CODE" -eq 413 ]; then
	echo SUCCESS 
else
	echo FAILED
fi

