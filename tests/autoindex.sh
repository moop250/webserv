#!/bin/bash

echo In autoindex test file

URL="http://127.0.0.1:8000/"

curl -s $URL > page.html

if grep -q "<title>Index of" page.html; then
	echo SUCCESS
else
	echo FAILED
fi
