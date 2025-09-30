////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////
// CONFIGURATION LOGIC UNIT EXPLAINED //
////////////////////////////////////////

Starting with an example,
	ideally prototype a server as :

"
	server {
		...
		...
		...
	}
"

It works by the following rules :

1) Define his range by '{}' as :
	server { ... }

2) Each attribute must be splitted by ';' as :
	server { ... ; ... }

3) There can be multiple servers as :
	server { ... } server { ... }

4) The server accepted attributes have to be formated as :
	- hosts	: "host"			    : '<token> host'
	- ports "listen"			    : '<token> port'
	- server name "server_name"		: '<token> name'
	- root path "root"			    : '<token> path'
	- index	"html_index"			: '<token> file'
	- autoindex "autoindex"			: '<token> ON | OFF' 
	- error pages "error_page"		: '<token> nb path'
	- storage "upload_storage"		: '<token> path'
	- CGI "CGI" 			    	: '<token> file.py | file.php | ... <space> path'
	- "client_max_body_size"	    : '<token> size'
	- Methods allowed "allow"		: '<token> M1 M2 ... Mi'
	- Redirections "return"			: '<token> code path'
	- location bloc	"location"		: 'token path { ... }'

5) The location bloc has to respect the same format as a server

6) There can be multiple location ports in a same server

7) The location accepted attributes are the same as server except :
	- hosts
	- listen
	- location
	- server name

8) If no argument is given to webserv a default configuration take place.

9) Same for the error pages and the methos allowed. If undefined
	methods allowed will be GET DELETE POS. If undefined error page
	a ours default pages will be thrown
 
...)
	...

N) If one of the precedent rules aren't respected, the programm won't launch
todo:
- Match location if apply.
- Preappend root or cgi path to URL path
<<<<<<< HEAD:webserv/README.md

////////////////////////////////////////////////////////////////////////////////
=======
>>>>>>> refs/remotes/origin/RequestParsing:README.md

Note:
To install in eval computer:
-- install php-cgi:  sudo apt-get install php-cgi
-- install java: sudo apt install default-jre

/////////////////////////////////////////////////////////////////////////////////

Now, our server config file relates a lot to these of nginx. But we changed a bit
the names of the tokens for a chill parsing. So if you want to use one config file
from one server to another : (eg : nginx.config --> webserv.config | contrary)
you can launch convert in the nginx directory.

This program works as the following : ./convert [fileName.config] [0 | 1]
0--> converts as : nginx.config --> webserv.config
1--> converts as : webserv.config --> nginx.config

//////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
//  # Request handler documentation:    //
//////////////////////////////////////////

## Connection class methods:
Getter:
- `getState()` -> Return the current state of the connection: 		
	READING_METHOD = 1,
	READING_PATH = 2,
	READING_HTTPVERSION = 3,
	READING_HEADERS = 4,
	READING_BODY = 5,
	READING_CHUNKED = 6,
	CONTINUE_READ = 7,
	MAKING_RESPONSE = 8,
	SENDING_RESPONSE = 9,
	WAITING_REQUEST = 10
- `getRequest()` -> Return the Request object the Connection is processing.
- `getResponse()` -> Return the Response object the Connection is processing.
- `getServer()` -> Return the RequestServer object the Connection is connecting to.
- `getChunkedSize()` -> Return the bytes left to read in long. Only use in `Transfer-Encoding: chunked` Header.
- `getOffset()` -> Return the bytes already sent by send() call. Only use when sending response. Default is -2.
- `getReconnect()` -> Return the reconnect state. It is false during the first connection to search for server. Subsequence connections are true so server won't be searched again.
- `getClose()` -> Return a boolean, if true then the connection will need to be closed.
- `getPort()` -> Return a std::string port.
- `getIP()` -> Return a std::string IP.

Setter:
- `setPort(const std::string port)` -> set port.
- `setIP(const std::string ip)` -> set ip.

Method:
- `minusOffset(long size)` -> Subtract size from the current offset.
- `plusOffset(long size)` -> Add size from the current offset.
- `clear()` -> Reset the connection (Request, Response, chunked_size, offset, buffer). Server is not reseted, state is set to WAITING_REQUEST and reconnect is set to true.



## Request class methods:
Getter:
- `getMethod()` 			-> Return the resquest method in std::string.
- `getPath()`				-> Return the request path in std::string.
- `getQuery()`				-> Return the query in std::string.
- `getHttpVersion()`		-> Return the request HTTP version in std::string.
- `getHeader(const std::string& key)`	-> Accept a key in std::string, return the value in std::string. If key doesn't exist, return an empty std::string istead. ATTENTION: Header keys are sanitized to lower case!
- `getCookie(const std::string& key)`	-> Accept a key in std::string, return the value in std::string. If key doesn't exist, return an empty std::string istead.
- `getCgiType()`			-> Return the request CGI type in std::string. The file type example is .py, .java, .js, etc. Complete list in /request/support_file.cpp
- `getRequestType()`		-> Return the request type in int. The request types are 0 = FILE, 1 = CGI, 2 = DIRECTORY and 3 = NONE.
- `getBody()`				-> Return the request body in case of POST method in std::string. If the request is different than POST or the body is empty. Return an empty std::string instead.
- `getContentLength()`		-> Return the Content-Length of the request in size_t.
- `getContentType()`		-> Return the Content-Type of the request in std::string.
- `getKeepAlive()`			-> Return the Connection header of the request in bool. ATTENTION: The value is sanitized to lower case -> keep-alive, close!
- `getKeepAliveTimeout()`	-> Return the keep alive timeout in int. Default is -1.
- `getKeepAliveMax()`		-> Return the keep alive max in int. Default is -1.
- `getHost()`				-> Return host (server_name) in std::string.
- `getPort()`				-> Return port in std::string.
- `getRedirect()`			-> Return the path to redirected to in std::string. If no redirect specified, return an empty string instead.

Method:
- `appendBody(const std::string& line)` -> Append line to the current body.


## Response class methods:
Getter:
- `getHttpVersion()` -> Return the response HTTP version in std::string.
- `getCode()` -> Return the response code in int.
- `getCodeMessage()` -> Return the response code message in std::string.
- `getHeader(const std::string& key)` -> Accept a key in std::string, return the value in std::string. If key doesn't exist, return an empty std::string istead.
- `getBody()` -> Return the response body in std::string.
- `getContentLength()` -> Return the response body Content-Length in int.
- `getContentType()` -> Return the response Content-Type in std::string.


Method:
- `headersToString()` -> Convert the response header into string. Return std::string.
- `constructResponse()` -> Construct a HTTP response from a response object. Return std::string.
- `sendResponse(int fd_client)` -> Send HTTP response.
///////////////////////////////////////////////////////////////////////////////////////////
