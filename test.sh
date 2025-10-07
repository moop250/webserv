#!/bin/bash

echo "///////////////////////////////////////////"
echo "//   Test de comparaison nginx / webserv //"
echo "///////////////////////////////////////////"
echo

rm -f logs/* || { echo no log files to remove; }


####################################################
#  Vérifications et définition du fichier config
####################################################

if (( $# == 0 )); then
    echo "Usage: $0 <config_file>"
    exit 1
fi

CONFIG_PATH="$PWD/""$1"

NAME=${1#configFiles/}

NAME=${NAME%.config}

echo "CONFIG_PATH = $CONFIG_PATH"
echo "NAME        = $NAME"

echo Name : $NAME

if [ ! -f "$CONFIG_PATH" ]; then
    echo "Erreur: fichier $CONFIG_PATH introuvable."
    exit 1
fi

# Vérifier que nginx, webserv et convert existent





####################################################
#  Création du répertoire logs
####################################################

LOG_DIR="./logs"
mkdir "$LOG_DIR"
NGINX_LOG="$LOG_DIR/nginxLogRequest.txt"
WEBSERV_LOG="$LOG_DIR/webservLogRequest.txt"

####################################################
#  Conversion config pour nginx
####################################################

./convert "$CONFIG_PATH" 0 # 0 webserv --> nginx and 1 for nginx --> webserv
cd configFiles ; mv *nginx* .. ; cd ..
if [ ! -f ""$NAME"_nginx.config" ] ; then
	echo La conversion a couille
	exit
fi

####################################################
#  Extraction server_name et port
####################################################

host_name=$(grep -m1 "<host>" "$CONFIG_PATH" | awk '{print $2}' | tr -d ';')
port=$(grep -m1 "listen" "$CONFIG_PATH" | awk '{print $2}' | tr -d ';')
search="${host_name}:${port}"

echo Host: $host_name
echo Port: $port
echo "Target: $search"
sleep 1
echo

####################################################
#  NGINX EXECUTION
####################################################

echo try nginx ? y/n

read -e line

if [ ! -f line "n" ] ; then
	echo "Execution nginx..."
	sleep 1 

	# --- GET tests ---
	echo "GET requests sur nginx..."
	
	# --- POST tests ---
	echo "POST requests sur nginx..."

    	echo "NGINX terminé."
	echo
else
	echo skipping nginx part
	sleep 1
fi

####################################################
#  WEBSERV EXECUTION
####################################################

echo "Execution webserv..."
cd webserv || { echo "Impossible d'entrer dans le répertoire webserv"; exit 1; }
make 
./webserv "$CONFIG_PATH" &
ws_pid=$!
cd ..
sleep 1

curl $search >> $WEBSERV_LOG

# --- GET tests ---
echo "GET requests sur webserv..."

curl -X GET $search

# --- POST tests ---
echo "POST requests sur webserv..."

kill $ws_pid

echo "Webserv terminé."
echo

####################################################
#  Comparaison des logs
####################################################

echo "Comparaison des logs..."
if diff -q "$WEBSERV_LOG" "$NGINX_LOG" >/dev/null; then
    echo "Les réponses sont identiques."
else
    echo "Differences détectees :"
    diff "$WEBSERV_LOG" "$NGINX_LOG" > ./logs/diff.log
fi

echo "Test finished"

rm -f *nginx*.config || { echo ;}
rm -f *webserv*.config || { echo ;}

