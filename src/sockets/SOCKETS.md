# Sockets & Poll Loop

The entire server runs on a single `poll()` call that watches every open file descriptor at once — server sockets, client sockets, and CGI pipes all live in the same `fds` vector. Each fd is tagged with a type (`SERVER`, `CLIENT`, `SYS_FD_IN`, `SYS_FD_OUT`, `CGI_FD_IN`) so the loop knows how to handle it.

## Startup

Each unique `host:port` pair from the config gets its own bound, listening socket. Those sockets are set to non-blocking with `fcntl(O_NONBLOCK)` and added to the poll set with `POLLIN`. Everything else starts from here.

## Accepting a connection

```
poll() fires POLLIN on a SERVER fd
  └─ type == SERVER → call accept()
       └─ new client fd returned
            └─ set non-blocking, add to poll set with POLLIN
            └─ create Connection object, state = READING_METHOD
```

Implemented in `handleConnection()` → `addToPollfd()`.

## Receiving a request

```
poll() fires POLLIN on a CLIENT fd
  └─ type == CLIENT → recv() into connection buffer
       ├─ 0 bytes  → client hung up, close + remove fd
       └─ n bytes  → feed buffer into request parser
            ├─ parser needs more data → stay on POLLIN, wait for next event
            └─ request complete → determine response type
                 ├─ static file / no I/O → state = MAKING_RESPONSE, switch fd to POLLOUT
                 └─ needs file I/O or CGI → open pipe/file fds, add them to poll set
```

Implemented in `handleClientData()`.

## File and CGI I/O

When a response requires reading a file or running a CGI script, auxiliary fds are registered alongside the client fd in the poll set. The client fd stays idle while the aux fds handle data transfer:

```
poll() fires POLLIN on a SYS_FD_IN / CGI_FD_IN fd
  └─ read available data into the connection's io buffer
       └─ when fd closes → state = MAKING_RESPONSE, remove aux fd

poll() fires POLLOUT on a SYS_FD_OUT
  └─ write request body into the pipe (e.g. POST body to CGI stdin)
       └─ when fully written → remove aux fd
```

The origin client fd for any aux fd is tracked via `ioFdMap` so responses always get routed back to the right connection.

A 3-second CGI timeout is checked on every loop iteration (`CGI_timeout()`). If a CGI process stalls, the connection is torn down cleanly.

## Sending a response

```
poll() fires POLLOUT on a CLIENT fd
  └─ state == MAKING_RESPONSE → build the full HTTP response string
  └─ state == SENDING_RESPONSE → send() up to 65000 bytes at a time
       ├─ bytes remaining → update offset, wait for next POLLOUT
       └─ fully sent → close fd, remove from poll set
```

Implemented in `handlePOLLOUT()`.

## Connection teardown

Any error, hangup (`POLLHUP`), or completed send closes the fd and removes it from the poll set along with any associated aux fds (`removeFromPollfd()`, `removeFromGenfd()`). A 300-second idle timeout is tracked per connection — if it fires, the next `POLLOUT` event closes the socket.

## FD type reference

| Type | Description |
|---|---|
| `SERVER` | Bound listening socket, one per unique `host:port` |
| `CLIENT` | Accepted client connection |
| `SYS_FD_IN` | File descriptor to read from (e.g. static file) |
| `SYS_FD_OUT` | File descriptor to write to (e.g. upload file) |
| `CGI_FD_IN` | CGI stdout pipe — read the script's response |
| `CGI_FD_OUT` | CGI stdin pipe — write the request body to the script |

## Source files

| File | Responsibility |
|---|---|
| `serverInitialization.cpp` | Parse config for unique host:port pairs, bind sockets, init poll set |
| `incomingConnection.cpp` | Main poll loop — dispatch on fd type and event |
| `genfd.cpp` | Add/remove auxiliary (file/CGI pipe) fds from the poll set |
| `SocketClass.cpp` | Low-level socket creation, binding, and address bookkeeping |
