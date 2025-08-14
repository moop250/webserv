# Request handler documentation:


## Request class methods:
Getter:
- `getMethod()` -> Return the resquest method in std::string.
- `getPath()` -> Return the request path in std::string.
- `getQuery()` -> Return the query in std::string.
- `getHttpVersion()` -> Return the request HTTP version in std::string.
- `getHeader()` -> Accept a key in std::string, return the value in std::string. If key doesn't exist, return an empty std::string istead.
- `getCookie()` -> Accept a key in std::string, return the value in std::string. If key doesn't exist, return an empty std::string istead.
- `getFileType()` -> Return the request file type in std::string. The file type example is py, html, css, etc.
- `getRequestType()` -> Return the request type in int. The request types are 0 = FILE, 1 = CGI, 2 = DIRECTORY and 3 = NONE.
- `getBody()` -> Return the request body in case of POST method in std::string. If the request is different than POST or the body is empty. Return an empty std::string instead.
- `getContentLength()` -> Return the Content-Length of the request in size_t.
