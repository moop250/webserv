#!/bin/bash

echo In client max body size testing file

URL="http://localhost:8080/upload"
LARGE_DATA=$(head -c 200000 </dev/urandom | base64)

HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" -X POST -d "$LARGE_DATA" $URL)
if [ "$HTTP_CODE" -eq 413 ]; then
	echo SUCCESS 
else
	echo FAILED
fi

