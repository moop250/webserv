#!/bin/bash

for i in {0..999}
do      
        curl 127.0.0.1:8001/html/accueil.html >> foo2.log
done  
