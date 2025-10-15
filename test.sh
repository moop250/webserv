#!/bin/bash


./tests/stress1.sh &
pid1=$!
./tests/stress2.sh &
pid2=$!


sleep 5 

kill $pid1
kill $pid2

#curl 127.0.0.1:8001/html/accueil.html
