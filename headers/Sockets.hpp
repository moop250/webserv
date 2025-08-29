#ifndef SOCKET_FUNCS_HPP
# define SOCKET_FUNCS_HPP

# include "SocketClass.hpp"
# include "Connection.hpp"
# include <poll.h>

ServerSocket *initalizeServer(Config *serverConfig);
std::vector<pollfd> initPoll(ServerSocket *socket);
int incomingConnection(ServerSocket *sockets, std::vector<pollfd> *fds, Config *config, char **env, std::map<int, Connection> *connectMap);

#endif // SOCKET_FUNCS_HPP