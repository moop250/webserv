#!/bin/bash

# =========================================
# Script de comparaison entre webserv et nginx
# =========================================

# Configurable
WEBSERV_URL="http://127.0.0.1:8001"
NGINX_URL="http://127.0.0.1:8080"

# Fichiers de test
TEST_PATHS=(
    "/html/accueil.html"
    "/html/error/404.html"
    "/api/test"
)

# Méthodes HTTP à tester
METHODS=("GET" "POST" "PUT")

# Dossier temporaire pour stocker les réponses
TMP_DIR=$(mktemp -d)
echo "Temp dir: $TMP_DIR"

# Fonction pour envoyer requête et sauvegarder réponse
send_request() {
    local URL=$1
    local METHOD=$2
    local FILE=$3

    curl -s -X "$METHOD" -w "%{http_code}" "$URL" -o "$FILE"
}

echo "======================================"
echo "Comparaison webserv vs Nginx"
echo "======================================"

for path in "${TEST_PATHS[@]}"; do
    echo ">>> Testing path: $path"
    for method in "${METHODS[@]}"; do
        # Webserv
        WS_FILE="$TMP_DIR/ws_$(echo $path | tr / _)_$method.txt"
        WS_CODE=$(send_request "$WEBSERV_URL$path" "$method" "$WS_FILE")
        
        # Nginx
        NG_FILE="$TMP_DIR/nginx_$(echo $path | tr / _)_$method.txt"
        NG_CODE=$(send_request "$NGINX_URL$path" "$method" "$NG_FILE")

        # Comparaison
        if [ "$WS_CODE" == "$NG_CODE" ]; then
            STATUS="OK"
        else
            STATUS="MISMATCH"
        fi

        echo "Method: $method | Webserv: $WS_CODE | Nginx: $NG_CODE | $STATUS"

        # Comparer le contenu si GET ou POST
        if [[ "$method" == "GET" || "$method" == "POST" ]]; then
            diff -q "$WS_FILE" "$NG_FILE" > /dev/null
            if [ $? -eq 0 ]; then
                echo "  Content: OK"
            else
                echo "  Content: DIFFERENT"
            fi
        fi
    done
done

# Nettoyage
# rm -rf "$TMP_DIR"
echo "Done. Responses stored in $TMP_DIR (for debugging)."

