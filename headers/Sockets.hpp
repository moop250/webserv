#ifndef SOCKET_FUNCS_HPP
# define SOCKET_FUNCS_HPP

# include "SocketClass.hpp"
# include "Connection.hpp"
# include <poll.h>


enum connectionReturns {
	ACCEPTERROR = -1,
	CONNECTIONSUCCESS
};

enum recvReturns {
	EXITPARSING = 0,
	HUNGUP = -1,
	RECVERROR = -2
};

enum fdTypes {
	SERVER,
	CLIENT
};

typedef struct s_fdInfo {

	std::vector<pollfd> fds;
	std::map<int, int> fdTypes;

} t_fdInfo;

ServerSocket *initalizeServer(Config *serverConfig);
void initPoll(ServerSocket *socket, t_fdInfo *fdInfo);
int incomingConnection(ServerSocket *sockets, t_fdInfo *fdInfo, Config *config, std::map<int, Connection> *connectMap);

#endif // SOCKET_FUNCS_HPP