#!/bin/bash

echo In index test file



URL="http://127.0.0.1:8000/"
CONTENT=$(curl -s $URL)

if echo "$CONTENT" | grep -qi "Welcome"; then
	echo SUCCESS
else
	echo FAILED
fi
