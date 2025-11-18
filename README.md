Petit rappel : Tout element de votre serveur devra se situer dans ressources

# CONFIGURATION LOGIC UNIT EXPLAINED #


##  Starting with an example,

ideally prototype a server as :

"
	server {
		...
		...
		...
	}
"

It works by the following rules :

##  1) Define his range by '{}' as :
	server { ... }

##  2) Each attribute must be splitted by ';' as :
	server { ... ; ... }

##  3) There can be multiple servers as :
	server { ... } server { ... }

##  4) The server accepted attributes have to be formated as :
	- hosts	: "host"			: '<token> host'
	- listeners "listen"			: '<token> port'
	- server name "server_name"		: '<token> name'
	- root path "root"			: '<token> path'
	- index	"html_index"			: '<token> file'
	- autoindex "autoindex"			: '<token> ON | OFF' 
	- error pages "error_page"		: '<token> nb path'
	- storage "upload_storage"		: '<token> path'
	- CGI "CGI" 				: '<token> file.py | file.php | ... <space> path'
	- client BufSize "client_max_body_size"	: '<token> size'
	- Methods allowed "allow"		: '<token> M1 M2 ... Mi'
	- Redirections "return"			: '<token> code path'
	- location bloc	"location"		: 'token path { ... }'

##  5) The location bloc has to respect the same format as a server

##  6) There can be multiple location ports in a same server

##  7) The location accepted attributes are the same as server except :
	- hosts
	- listen
	- location
	- server name

##  8) If no argument is given to webserv a default configuration take place.

##  9) Same for the error pages and the methos allowed. If undefined
	methods allowed will be GET DELETE POS. If undefined error page
	a 404 not found will be throw (i think...)
 
...)
	...

##  N) If one of the precedent rules aren't respected, the programm won't launch
todo:
- Match location if apply.
- Preappend root or cgi path to URL path

Note:
To install in eval computer:
-- install php-cgi:  sudo apt-get install php-cgi
-- install java: sudo apt install default-jre


# Convert NGINX to WEBSERV files.conf

Now, our server config file relates a lot to these of nginx. But we changed a bit
the names of the tokens for a chill parsing. So if you want to use one config file
from one server to another : (eg : nginx.config --> webserv.config | contrary)
you can launch convert in the nginx directory.

This program works as the following : ./convert [fileName.config] [0 | 1]
0--> converts as : nginx.config --> webserv.config
1--> converts as : webserv.config --> nginx.config

# TESTER WEBSERV

##  Utiliser test.sh

Les verifications automatiques (SUCCESS & FAILED) ne sont valides que pour default.config

