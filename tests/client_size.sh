#!/bin/bash

echo In client max body size testing file 
echo Il faut modifier config file avec client_max_body_size a 10 par exemple

URL="127.0.0.1:8001"
LARGE_DATA="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv"

HTTP_CODE=$(curl -X POST "$LARGE_DATA" $URL)

echp $HTTP_CODE
if [ "$HTTP_CODE" -eq 413 ]; then
	echo SUCCESS 
else
	echo FAILED
fi

