#!/bin/bash

echo bravo, tu veux tester si le code est bon !

#curl --trace-ascii debugdump.txt http://polyception.ch

#host
#curl --resolve polyception.ch:80:127.0.0.1 http://polyception.ch

#port number
#curl http://polyception.ch:1234 # idk how it works

# GET & POST
#curl https://curl.se
# with headers
#curl https://curl.se --head

# it sends first a head then a get
#curl -I http://example.com --next http://example.com

# it sends a POST then a GET
#curl -d score=10 http://example.com/post.cgi --next http://example.com/results.html

# it's prototype as : 
# <form method="GET" action="junk.cgi">
#  <input type=text name="birthyear">
#  <input type=submit name=press value="OK">
#</form>
#
#and
#<form method="POST" action="junk.cgi">
#  <input type=text name="birthyear">
#  <input type=submit name=press value=" OK ">
#</form>


