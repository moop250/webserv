#!/bin/bash

echo ///////////////////////////////////////////
echo //   test de comparaison nginx webserv   //
echo ///////////////////////////////////////////

echo

####################################################################

#   ///////////////
#-- NGINX EXECUTION --#
#   ///////////////

echo Execution nginx
# convert .config if necessary

echo hosts:ports in action : 
cat $BASH_ARGV | grep server_name > tmp_serv_name.txt
cat $BASH_ARGV | grep listen > tmp_port.txt
export server_name=$(cat tmp_serv_name.txt)
export port=$(cat tmp_port.txt)
echo $server_name:$port
# give $BASH_ARGV (the configuration file) to nginx server 

curl --trace-ascii nginxLogRequest.txt 127.0.0.1:port


####################################################################

#    ///////////////
#-- WEBSERV EXECUTION --#
#    ///////////////

echo Execution webserv
# convert .config if necessary

cd ../ ; make re
cd $OLDPWD
ws_pid=./webserv $BASH_ARGV &

######################################################################


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

# pas utile je crois
#curl --data "<xml>" --header "Content-Type: text/xml" --request PROPFIND polyception.ch


m
