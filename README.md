### Request handler documentation:


## Request class methods:
# Getter:
- `getMethod()` -> Return the resquest method in std::string.
- `getPath()` -> Return the request path in std::string.
- `getQuery()` -> Return the query in std::string.
- `getHttpVersion()` -> Return the request HTTP version in std::string.
- `getHeader(const std::string& key)` -> Accept a key in std::string, return the value in std::string. If key doesn't exist, return an empty std::string istead.
- `getCookie(const std::string& key)` -> Accept a key in std::string, return the value in std::string. If key doesn't exist, return an empty std::string istead.
- `getCgiType()` -> Return the request CGI type in std::string. The file type example is .py, .java, .js, etc. Complete list in /request/support_file.cpp
- `getRequestType()` -> Return the request type in int. The request types are 0 = FILE, 1 = CGI, 2 = DIRECTORY and 3 = NONE.
- `getBody()` -> Return the request body in case of POST method in std::string. If the request is different than POST or the body is empty. Return an empty std::string instead.
- `getContentLength()` -> Return the Content-Length of the request in size_t.
- `getContentType()` -> Return the Content-Type of the request in std::string.


## Response class methods:
# Getter:
- `getHttpVersion()` -> Return the response HTTP version in std::string.
- `getCode()` -> Return the response code in int.
- `getCodeMessage()` -> Return the response code message in std::string.
- `getErrorPagePath()` -> Return the path to an error page.
- `getHeader(const std::string& key)` -> Accept a key in std::string, return the value in std::string. If key doesn't exist, return an empty std::string istead.
- `getBody()` -> Return the response body in std::string.
- `getContentLength()` -> Return the response body Content-Length in int.
- `getContentType()` -> Return the response Content-Type in std::string.

# Method:
- `headersToString()` -> Convert the response header into string. Return std::string.
- `constructResponse()` -> Construct a HTTP response from a response object. Return std::string.