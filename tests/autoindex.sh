#!/bin/bash

echo In autoindex test file

URL="http://localhost:8080/dir_without_index/"

curl -s $URL > page.html

if grep -q "<title>Index of" page.html; then
	echo SUCCESS
else
	echo FAILED
fi
