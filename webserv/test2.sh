#!/bin/bash

for i in {1..100000}
do
	echo Requete N* $i >> log2.log
	curl 127.0.0.1:8001/html/accueil.html >> log2.log
done
