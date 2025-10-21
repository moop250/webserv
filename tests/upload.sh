#!/bin/bash

echo In upload test file


URL="http://localhost:8080/upload"
FILE="test_upload.txt"
echo "This is a test file" > $FILE

RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" -X POST -F "file=@$FILE" $URL)
if [ "$RESPONSE" -eq 200 ] || [ "$RESPONSE" -eq 201 ]; then
	echo SUCCESS
else
	echo FAILED
fi
rm -f $FILE
