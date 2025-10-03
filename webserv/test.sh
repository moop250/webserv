#!/bin/bash

./test1.sh &
pid1=$!
./test2.sh &
pid2=$!
sleep 5
kill $pid1
kill $pid2
