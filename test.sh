#!/bin/bash

echo "///////////////////////////////////////////"
echo "//   Test de comparaison nginx / webserv //"
echo "///////////////////////////////////////////"
echo

####################################################
#  Vérifications et définition du fichier config
####################################################

if (( $# == 0 )); then
    echo "Usage: $0 <config_file>"
    exit 1
fi

CONFIG_PATH=$PWD/$1
CONFIG_BASENAME=$(basename "$CONFIG_PATH" .config)

echo $CONFIG_BASENAME
if [ ! -f "$CONFIG_PATH" ]; then
    echo "Erreur: fichier $CONFIG_PATH introuvable."
    exit 1
fi

# Vérifier que nginx, webserv et convert existent





####################################################
#  Création du répertoire logs
####################################################

LOG_DIR="./logs"
mkdir -p "$LOG_DIR"
NGINX_LOG="$LOG_DIR/nginxLogRequest.txt"
WEBSERV_LOG="$LOG_DIR/webservLogRequest.txt"

####################################################
#  Conversion config pour nginx
####################################################

./convert "$CONFIG_PATH" 0 # 0 webserv --> nginx and 1 for nginx --> webserv
cd configFiles ; mv *nginx* .. ; cd ..
NGINX_CONFIG="${CONFIG_BASENAME}_nginx.config"
if [ ! -f "$NGINX_CONFIG" ]; then
    echo "Erreur : la conversion a échoué."
    exit 1
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
	nginx -c "$(realpath "$NGINX_CONFIG")" -p "$(pwd)" &
	nginx_pid=$!
	sleep 1 

	# --- GET tests ---
	echo "GET requests sur nginx..."
	
	# --- POST tests ---
	echo "POST requests sur nginx..."
	
	#	faut checker dans proc si le processs (ps) tourne ou pas sinon crash sh
	kill $nginx_pid
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

curl $search

# --- GET tests ---
echo "GET requests sur webserv..."

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
    echo "Différences détectées :"
    diff -u "$WEBSERV_LOG" "$NGINX_LOG" | less
fi

echo "Test terminé."

