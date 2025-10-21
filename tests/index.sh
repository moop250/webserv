#!/bin/bash

echo In index test file



URL="http://localhost:8080/"
CONTENT=$(curl -s $URL)

if echo "$CONTENT" | grep -qi "Welcome"; then
	echo SUCCESS
else
	echo FAILED
fi
