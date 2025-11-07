#!/bin/bash

iter=0
for i in {0..999}
do
	iter=$i
	sleep 0.1	
        curl 127.0.0.1:8001/html/accueil.html >> foo2.log
	echo Page getted : $iter times > foo2.log
done  
