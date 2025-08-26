#ifndef SOCKET_FUNCS_HPP
# define SOCKET_FUNCS_HPP

# include "SocketClass.hpp"

ServerSocket *initalizeServer(Config *serverConfig);
struct pollfd *initPoll(ServerSocket *socket);
int incomingConnection(ServerSocket *sockets, struct pollfd **fds, Config *config, char **env);

#endif // SOCKET_FUNCS_HPP