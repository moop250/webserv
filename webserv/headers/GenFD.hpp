#ifndef GENFD_HPP
# define GENFD_HPP

#include "SocketClass.hpp"
#include "Sockets.hpp"
#include "Connection.hpp"
#include "request_handler.hpp"
#include <cstdio>
#include <string>

int handleFDIn(t_fdInfo *fdInfo, int fd, Connection *connect);
int handleFDOut(t_fdInfo *fdInfo, int fd, Connection *connect);

#endif