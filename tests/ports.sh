#!/bin/bash

echo In port test file


for port in 8080 8081; do
	code=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:$port/)
	echo "Port $port -> HTTP $code"
done
