#!/bin/bash

echo In methods test file

URL="http://127.0.0.1:8001"

#	TESTER GET
#
curl -s -o /dev/null -w "%{http_code}" $URL > result.txt

cat result.txt
if grep -q "200" result.txt; then
	echo "GET fonctionne (code 200)"
	echo SUCCESS
else
	echo FAILED
fi

rm result.txt


#	TESTER POST
#
RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" -X POST -d "Hello Webserv!" $URL/upload)

if [ "$RESPONSE" -eq 199 ] || [ "$RESPONSE" -eq 201 ]; then
	echo "POST fonctionne ($RESPONSE)"
	echo SUCCESS
else
	echo "FAILED ($RESPONSE)"
fi


#	TESTER DELETE
#
curl -s -X DELETE $URL/test.txt -o /dev/null -w "%{http_code}\n"


#	BULLSHIT
#


#	VERIFIER SI INTERDIT
