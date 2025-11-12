#ifndef GENFD_HPP
# define GENFD_HPP

#include "SocketClass.hpp"
#include "Sockets.hpp"
#include "Connection.hpp"
#include "request_handler.hpp"
#include <cstdio>
#include <string>

int		handleFDIn(int fd, Connection *connect);
int		handleFDOut(int fd, Connection *connect);
void	addToGenFD(t_fdInfo *fdInfo, int newFD, int originFD, int fdType);
void    removeFromGenfd(t_fdInfo *fdInfo, int fd);

#endif