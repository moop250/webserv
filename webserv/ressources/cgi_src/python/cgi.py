#!/usr/bin/env python3
#import cgi

print("Content-Type: text/html\r\n\r\n")
print("<html><head><title>CGI Python</title></head>")
print("<body>")
print("<h1>Bonjour, monde !</h1>")
# Ce script peut aussi lire les données d'un formulaire
#form = cgi.FieldStorage()
#if "nom" in form:
#    nom_utilisateur = form["nom"].value
#    print(f"<p>Bienvenue, {nom_utilisateur} !</p>")
print("</body></html>")