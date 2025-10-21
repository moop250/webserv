#!/bin/bash

echo In autoindex test file

URL="http://localhost:8080/"

curl -s $URL > page.html

if grep -q "<title>Index of" page.html; then
	echo SUCCESS
else
	echo FAILED
fi
