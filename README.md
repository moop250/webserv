# webserv
![](https://img.shields.io/badge/125%2F100-brightgreen)

An HTTP/1.1 web server written in C++98, built as part of the 42 curriculum.

## Description

webserv is a non-blocking HTTP server that handles multiple simultaneous connections using a single `poll()` loop. It supports serving static files, file uploads, directory listing, HTTP redirects, and CGI execution — all configurable through an nginx-inspired configuration file.

## Core Features

- Non-blocking I/O via a single `poll()` loop monitoring all server sockets, client connections, and CGI pipes simultaneously
- Static file serving with automatic content-type detection
- Directory listing (autoindex)
- File upload (POST) and deletion (DELETE)
- Chunked transfer encoding decoding
- Custom error pages with built-in fallbacks
- HTTP redirects (301/302)
- Virtual hosting: multiple server blocks, each with their own host/port/server_name
- Location blocks for per-route configuration overrides

## Bonus Features

- CGI execution (any interpreter, mapped by file extension) with fork/pipe and a 3-second timeout
- Cookie parsing and session management
- Multiple concurrent CGI handlers

## Compatibility & Size

![](https://img.shields.io/badge/Linux-0a97f5?style=for-the-badge&logo=linux&logoColor=white)

![](https://img.shields.io/github/languages/code-size/moop250/webserv)
-

## Building

```bash
make        # build the webserv binary
make re     # clean rebuild
make clean  # remove object files
make fclean # remove objects and binary
```

No external libraries required.

## Usage

```bash
./webserv [configuration file]
```

Example configs are provided in [ressources/configFiles/](ressources/configFiles/):

```bash
./webserv ressources/configFiles/test_default.conf
```

## Configuration

The format is inspired by NGINX's `server` block. Multiple `server` blocks are supported; the first server defined for a given `host:port` acts as the default for that address.

```nginx
server {
    <listen>               8080;
    <host>                 127.0.0.1;
    <server_name>          localhost;
    <root>                 ressources/;
    <html_index>           /html/index.html;
    <upload_storage>       /upload;
    <allow>                GET POST DELETE;
    <autoindex>            ON;
    <client_max_body_size> 10;

    <error_page>           404 html/error/404.html;
    <error_page>           500 html/error/500.html;

    <CGI>                  .py cgi_src/python/;

    location /api/ {
        <root>             ressources/;
        <allow>            GET POST;
    }

    <return>               301 /new-location;
}
```

### Directives

| Directive | Description |
|---|---|
| `<listen>` | Port to listen on |
| `<host>` | IP address to bind |
| `<server_name>` | Virtual host name |
| `<root>` | Document root directory |
| `<html_index>` | Default file to serve for directory requests |
| `<allow>` | Whitelist of accepted HTTP methods |
| `<autoindex>` | Enable (`ON`) or disable (`OFF`) directory listing |
| `<client_max_body_size>` | Maximum request body size in bytes |
| `<error_page>` | Custom error page: `<code> <path>` |
| `<CGI>` | CGI handler mapped to a file extension: `<.ext> <handler-dir>` |
| `<upload_storage>` | Directory where uploaded files are saved |
| `<return>` | HTTP redirect: `<code> <target>` |
| `location /path/ { }` | Override any directive for a specific path prefix |

## Project Structure

```
.
├── src/
│   ├── main.cpp
│   ├── configSetUp/    # configuration file parsing and validation
│   ├── request/        # HTTP request parsing and connection state machine
│   ├── response/       # response building, file serving, CGI execution
│   ├── sockets/        # socket setup, poll loop, connection acceptance
│   └── utils/          # error handling, debug logging
├── headers/            # all header files
└── ressources/
    ├── configFiles/    # example configuration files
    ├── cgi_src/        # example CGI scripts
    ├── html/           # static HTML pages and error pages
    └── upload/         # default upload directory
```
