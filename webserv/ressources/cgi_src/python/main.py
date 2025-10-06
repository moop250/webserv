#!/usr/bin/python3

import os
import cgitb
cgitb.enable()

print("Content-Type: text/html\r\n\r\n")
print("<html><body><h1>Hello depuis Python CGI !</h1></body></html>")

print("LALALALALLALALALAL")
print()
print()
print()
print()
print()


print("Content-Type: text/html\r\n")
print("\r\n")

print("<html><body>")
print("<h1>CGI Python OK </h1>")
print("<p>QUERY_STRING = {}</p>".format(os.environ.get("QUERY_STRING", "")))
print("</body></html>")