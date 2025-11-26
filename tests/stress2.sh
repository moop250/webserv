#!/bin/bash

if [ -f "foo2.log" ] ; then
	rm foo2.log
fi
for i in {0..999}
do
  	echo iteration : $i >> foo2.log	
        curl --silent 127.0.0.1:8001/html/accueil.html >> foo2.log
done  
