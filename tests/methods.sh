#!/bin/bash

echo In methods test file

URL="http://127.0.0.1:8001"

#	TESTER GET
#
curl -s -o /dev/null -w "%{http_code}" $URL > result.txt

cat result.txt
if grep -q "200" result.txt; then
	echo "GET fonctionne"
	echo SUCCESS
else
	echo "FAILED ("
fi

rm result.txt


#	TESTER POST
#
echo Testing post
RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" -X POST -d "Hello Webserv!" $URL/upload)

if [ "$RESPONSE" -eq 200 ] || [ "$RESPONSE" -eq 201 ]; then
	echo "POST fonctionne ($RESPONSE)"
	echo SUCCESS
else
	echo "POST REQUEST FAILED (STATUS CODE : $RESPONSE)"
fi


#	TESTER DELETE
#
RE=$(curl -s -X DELETE $URL/test.txt -o /dev/null -w "%{http_code}\n")
echo for delete : "(STATUS CODE : $RE)"

#	BULLSHIT
#


#	VERIFIER SI INTERDIT
