#!/bin/bash

echo "///////////////////////////////////////////"
echo "//   Test de comparaison nginx / webserv //"
echo "///////////////////////////////////////////"
echo

####################################################
#  Vérifications et définition du fichier config
####################################################

if [ $# -eq 0 ]; then
    echo "Usage: $0 <config_file>"
    exit 1
fi

CONFIG_PATH="$(realpath "$1")"
CONFIG_BASENAME=$(basename "$CONFIG_PATH" .config)

if [ ! -f "$CONFIG_PATH" ]; then
    echo "Erreur: fichier $CONFIG_PATH introuvable."
    exit 1
fi

# Vérifier que nginx, webserv et convert existent
command -v nginx >/dev/null 2>&1 || { echo "nginx non trouvé"; exit 1; }
[ -x ./webserv/webserv ] || { echo "./webserv introuvable"; exit 1; }
[ -x ./convert ] || { echo "./convert introuvable"; exit 1; }

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

server_name=$(grep -m1 "host" "$CONFIG_PATH" | awk '{print $2}' | tr -d ';')
port=$(grep -m1 "listen" "$CONFIG_PATH" | awk '{print $2}' | tr -d ';')
search="${server_name}:${port}"

echo "Target: $search"
echo

####################################################
#  NGINX EXECUTION
####################################################

echo "Execution nginx..."
nginx -c "$(realpath "$NGINX_CONFIG")" -p "$(pwd)" &
nginx_pid=$!
sleep 1  # laisser le temps au serveur de démarrer

# --- GET tests ---
echo "GET requests sur nginx..."
> "$NGINX_LOG"
curl -s -o /dev/null -w "%{http_code}\n" "$search/index.html" >> "$NGINX_LOG"
for i in $(seq 1 5); do
    curl -s "$search/file$i.txt" >> "$NGINX_LOG"
done

# --- POST tests ---
echo "POST requests sur nginx..."
curl -s -X POST -d "foo=bar" "$search/form" >> "$NGINX_LOG"

# Kill nginx
if [ -n "$nginx_pid" ] && ps -p $nginx_pid > /dev/null; then
    kill $nginx_pid
    sleep 1
fi
echo "NGINX terminé."
echo

####################################################
#  WEBSERV EXECUTION
####################################################

echo "Execution webserv..."
cd webserv || { echo "Impossible d'entrer dans le répertoire webserv"; exit 1; }
make 
./webserv "$CONFIG_PATH" &
ws_pid=$!
cd ..
sleep 1  # laisser le temps au serveur de démarrer

# --- GET tests ---
echo "GET requests sur webserv..."
> "$WEBSERV_LOG"
curl -s -o /dev/null -w "%{http_code}\n" "$search/index.html" >> "$WEBSERV_LOG"
for i in $(seq 1 5); do
    curl -s "$search/file$i.txt" >> "$WEBSERV_LOG"
done

# --- POST tests ---
echo "POST requests sur webserv..."
curl -s -X POST -d "foo=bar" "$search/form" >> "$WEBSERV_LOG"

# Kill webserv
if [ -n "$ws_pid" ] && ps -p $ws_pid > /dev/null; then
    kill $ws_pid
    sleep 1
fi
echo "Webserv terminé."
echo

####################################################
#  Comparaison des logs
####################################################

echo "Comparaison des logs..."
if diff -q "$WEBSERV_LOG" "$NGINX_LOG" >/dev/null; then
    echo "✅ Les réponses sont identiques."
else
    echo "❌ Différences détectées :"
    diff -u "$WEBSERV_LOG" "$NGINX_LOG" | less
fi

echo "Test terminé."

