#!/bin/bash

for i in {0..10000} 
do
	echo requete $i >> log1.log
	curl 127.0.0.1:8001/html/accueil.html >> log1.log
done
