#!/bin/bash

echo In upload test file


URL="http://127.0.0.1:8001/upload"
FILE="test_upload.txt"
echo "This is a test file" > $FILE

rm -f $FILE
