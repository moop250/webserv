#!/bin/bash

HOST="127.0.0.1"
PORT="8001"
BASE_URL="http://$HOST:$PORT/test"

GREEN="\e[32m"
RED="\e[31m"
YELLOW="\e[33m"
RESET="\e[0m"

declare -a TESTS=(
  "301_redirect 301 Moved_Permanently"
  "302_redirect 302 Found"
  "403_forbidden 403 Forbidden"
  "404_missing/doesnotexist.txt 404 Not_Found"
  "405_method 405 Method_Not_Allowed"
  "411_length_required 411 Length_Required"
  "413_too_large 413 Content_Too_Large"
  "415_wrong_media/weird.bin 415 Unsupported_Media_Type"
  "500_internal_error/crash.py 500 Internal_Error"
  "501_not_implemented 501 Not_Implemented"
  "505_http_version 505 HTTP_Version_Mismatch"
  "204_empty/empty.txt 204 No_Content"
)

for test in "${TESTS[@]}"; do
	set -- $test
	PATH_REL=$1
	EXPECTED=$2
	LABEL=$3

	URL="$BASE_URL/$PATH_REL"
	curl -s -o /dev/null -w "%{http_code}" $URL
	CODE=$(curl -s -o /dev/null -w "%{http_code}" $URL)

	if [ "$CODE" == "$EXPECTED" ]; then
		echo -e "${GREEN} $LABEL -> $CODE${RESET}"
	else
		echo -e "${RED} $LABEL -> $CODE (attendu $EXPECTED)${RESET}"
	fi
done

echo "---------------------------------------------------------"

