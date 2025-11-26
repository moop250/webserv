#!/bin/bash

if [ -f "foo1.log" ] ;
then rm foo1.log
fi

for i in {0..999}
do
	echo iteration : $i >> foo1.log
	curl --silent 127.0.0.1:8001/html/accueil.html >> foo1.log
done
